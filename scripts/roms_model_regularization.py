import argparse
import toml
import os
import pyproj
import numpy as np
from netCDF4 import Dataset
import xroms
import scipy
import matplotlib.pyplot as plt
import xarray as xr
import cartopy
import time

def main(args):
    
    ds = xr.open_mfdataset(args.model_path, compat='override', combine='by_coords',
                      data_vars='minimal', coords='minimal', parallel=True, chunks={'ocean_time':1})
    ds, xgrid = xroms.roms_dataset(ds, include_cell_volume=True, include_Z0=True)
    ds.xroms.set_grid(xgrid)

    if (not (ds["lat_rho"][0,0] == ds["lat_rho"][0,:]).all() or
       not (ds["lon_rho"][0,0] == ds["lon_rho"][:,0]).all()):
        raise NotImplementedError("The lat/lon grid is not north aligned, so it needs to be interpolated to north aligned, but this is not implemented yet")

    #interpolate u,v,w to rho points and rotate them to be north/east facing instead of eta/xi facing
    ds['w'] = xroms.to_grid(ds.w, xgrid, hcoord='rho', scoord='s_rho')
    ds['u'], ds['v'] = xroms.vector.rotate_vectors(ds['u'], ds['v'], ds['angle'], xgrid=xgrid)

    data_fields_to_save = set(["u", "v", "w", "temp", "salt", "dye_01", "ocean_time", "s_rho", "lon_rho", "lat_rho", "h", "z_rho0"])
    
    data_fields_to_drop = set(ds.variables.keys()) - data_fields_to_save

    ds = ds.drop_vars(data_fields_to_drop)

    _, datasets = zip(*ds.groupby("ocean_time"))
    paths = []
    for p in args.model_path:
        paths.append(os.path.join(args.output_path, os.path.basename(p)))
        
    print("Save Regularized NetCDF Model Files")
    xr.save_mfdataset(datasets, paths, format="NETCDF3_64BIT")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("output_path", help="The path to output model files")
    parser.add_argument('-p', '--model-path', nargs='+', default=[])


    args = parser.parse_args()
    main(args)
