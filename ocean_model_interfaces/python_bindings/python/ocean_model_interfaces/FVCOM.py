import ctypes

class ModelData(ctypes.Structure):
    _fields_ = [('u',ctypes.c_double),
                ('v',ctypes.c_double),
                ('w',ctypes.c_double),
                ('temp',ctypes.c_double),
                ('salt',ctypes.c_double),
                ('dye',ctypes.c_double),
                ('depth',ctypes.c_double)]

class FVCOM:
    def __init__(self, filename):
        ctypes.CDLL('/usr/local/lib/libocean_model_interfaces.so') #For some reason python can't find this by itself.
        self.lib = ctypes.CDLL('/usr/local/lib/libocean_model_interfaces_py.so')
        self.filename = filename

        utf8_filename = filename.encode('utf-8')

        self.get_data_fvcom = self.lib.getDataFVCOM
        self.get_data_fvcom.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
        self.get_data_fvcom.restype = ModelData

        self.get_data_out_of_range_fvcom = self.lib.getDataOutOfRangeFVCOM
        self.get_data_out_of_range_fvcom.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
        self.get_data_out_of_range_fvcom.restype = ModelData

        self.new_fvcom = self.lib.newFVCOM
        self.new_fvcom.argtypes = [ctypes.c_char_p]
        self.new_fvcom.restype = ctypes.c_void_p

        self.del_fvcom = self.lib.deleteFVCOM
        self.del_fvcom.argtypes = [ctypes.c_void_p]
        self.del_fvcom.restype = None

        self.handle = self.new_fvcom(utf8_filename)

    def __del__(self):
        self.del_fvcom(self.handle)

    def get_data(self, x, y, z, time):
        x = float(x)
        y = float(y)
        z = float(z)
        time = float(time)
        data_dict = {}
        data = self.get_data_fvcom(self.handle, x, y, z, time)
        
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