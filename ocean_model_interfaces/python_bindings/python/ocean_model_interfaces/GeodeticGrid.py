import ctypes
from  ocean_model_interfaces import ModelInterface

class ModelData(ctypes.Structure):
    _fields_ = [('u',ctypes.c_double),
                ('v',ctypes.c_double),
                ('w',ctypes.c_double),
                ('temp',ctypes.c_double),
                ('salt',ctypes.c_double),
                ('dye',ctypes.c_double),
                ('depth',ctypes.c_double)]

class GeodeticGridParameters(ctypes.Structure):
    _fields_ = [('modelDirectory',ctypes.c_char_p),
                ('timeChunkSize',ctypes.c_uint),
                ('depthChunkSize',ctypes.c_uint),
                ('latChunkSize',ctypes.c_uint),
                ('lonChunkSize',ctypes.c_uint),
                ('cacheSize',ctypes.c_uint)]

    def __init__(self, modelDirectory, timeChunkSize=10, depthChunkSize=10, latChunkSize=10, lonChunkSize=10, cacheSize=10):
        super().__init__()
        self.modelDirectory = ctypes.c_char_p(modelDirectory.encode('utf-8'))
        self.timeChunkSize = ctypes.c_uint(timeChunkSize)
        self.depthChunkSize = ctypes.c_uint(depthChunkSize)
        self.latChunkSize = ctypes.c_uint(latChunkSize)
        self.lonChunkSize = ctypes.c_uint(lonChunkSize)
        self.cacheSize = ctypes.c_uint(cacheSize)

class GeodeticGrid(ModelInterface.ModelInterface):
    def __init__(self, parameters):
        super().__init__()

        self.parameters = parameters

        self.get_data_geodetic_grid = self.lib.getDataGeodeticGrid
        self.get_data_geodetic_grid.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
        self.get_data_geodetic_grid.restype = ModelData

        self.get_data_out_of_range_geodetic_grid = self.lib.getDataOutOfRangeGeodeticGrid
        self.get_data_out_of_range_geodetic_grid.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
        self.get_data_out_of_range_geodetic_grid.restype = ModelData

        self.new_geodetic_grid = self.lib.newGeodeticGrid
        self.new_geodetic_grid.argtypes = [GeodeticGridParameters]
        self.new_geodetic_grid.restype = ctypes.c_void_p

        self.del_geodetic_grid = self.lib.deleteGeodeticGrid
        self.del_geodetic_grid.argtypes = [ctypes.c_void_p]
        self.del_geodetic_grid.restype = None

        self.handle = self.new_geodetic_grid(self.parameters)

    def __del__(self):
        if self.handle is not None:
            self.del_geodetic_grid(self.handle)

    def get_data(self, x, y, z, time):
        x = float(x)
        y = float(y)
        z = float(z)
        time = float(time)
        data_dict = {}
        data = self.get_data_geodetic_grid(self.handle, x, y, z, time)
        
        data_dict["u"] = data.u
        data_dict["v"] = data.v
        data_dict["w"] = data.w
        data_dict["temp"] = data.temp
        data_dict["salt"] = data.salt
        data_dict["dye"] = data.dye
        data_dict["depth"] = data.depth

        return data_dict

    def get_data_out_of_range(self, x, y, z, time):
        x = float(x)
        y = float(y)
        z = float(z)
        time = float(time)
        data_dict = {}
        data = self.get_data_out_of_range_fvcom(self.handle, x, y, z, time)
        
        data_dict["u"] = data.u
        data_dict["v"] = data.v
        data_dict["w"] = data.w
        data_dict["temp"] = data.temp
        data_dict["salt"] = data.salt
        data_dict["dye"] = data.dye
        data_dict["depth"] = data.depth

        return data_dict