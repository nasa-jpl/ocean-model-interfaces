
import ocean_model_interfaces

fvcom = ocean_model_interfaces.FVCOM("../../test_data/axial_data_test")

print(fvcom.get_data(-96.5869768, 50.2484645, -70, 0))