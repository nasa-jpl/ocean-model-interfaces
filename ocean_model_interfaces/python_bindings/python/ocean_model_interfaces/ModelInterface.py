import ctypes
from enum import IntEnum

class CoordinateType(IntEnum):
    XY = 0
    LATLON = 1

    @classmethod
    def from_param(cls, obj):
        if not isinstance(obj, cls):
            raise TypeError("Argument must be a CoordinateType enum instance")
        return ctypes.c_int(obj.value)


class ModelInterface:
    def __init__(self):
        try:
            ctypes.CDLL('/usr/local/lib/libocean_model_interfaces.so') #For some reason python can't find this by itself.
            self.lib = ctypes.CDLL('/usr/local/lib/libocean_model_interfaces_py.so')
        except OSError:
            ctypes.CDLL('/usr/local/lib/libocean_model_interfaces.dylib') #For some reason python can't find this by itself.
            self.lib = ctypes.CDLL('/usr/local/lib/libocean_model_interfaces_py.dylib')

        self.handle = None

        self.set_offsets_cpp = self.lib.setOffsets
        self.set_offsets_cpp.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
        self.set_offsets_cpp.restype = None

        self.set_origin_cpp = self.lib.setOrigin
        self.set_origin_cpp.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double]
        self.set_origin_cpp.restype = None

        self.set_coordinate_type_cpp = self.lib.setCoordinateType
        self.set_coordinate_type_cpp.argtypes = [ctypes.c_void_p, CoordinateType]
        self.set_coordinate_type_cpp.restype = None


    def set_offsets(self, offsetX, offsetY, offsetZ, offsetTime):
        self.set_offsets_cpp(self.handle, offsetX, offsetY, offsetZ, offsetTime)

    def set_origin(self, origin_longitude, origin_latitude):
        self.set_origin_cpp(self.handle, origin_longitude, origin_latitude)

    def set_coordinate_type(self, coordinate_type):
        self.set_coordinate_type_cpp(self.handle, coordinate_type)
