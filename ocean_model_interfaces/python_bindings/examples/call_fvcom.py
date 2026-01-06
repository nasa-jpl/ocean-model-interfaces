
from ocean_model_interfaces import GeodeticGrid, FVCOM, CoordinateType, GeodeticGridParameters

print("FVCOM")
fvcom = FVCOM("../../test_data/axial_data_test")
fvcom.set_origin(-160.0, 14.0)

fvcom.set_coordinate_type(CoordinateType.XY)
print(fvcom.get_data(-96.5869768, 50.2484645, -1000, 0))
print(fvcom.get_data(0, 0, -1000, 0))

fvcom.set_coordinate_type(CoordinateType.LATLON)
print(fvcom.get_data(-160.0, 14.0, -1000, 0))