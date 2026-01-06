import argparse
import os
import xroms
import xarray as xr

def main(args):
    
    ds = xr.open_mfdataset(args.model_files, compat='override', combine='by_coords',
                      data_vars='minimal', coords='minimal', parallel=True, chunks={'ocean_time':1})
    ds, xgrid = xroms.roms_dataset(ds, include_cell_volume=True, include_Z0=True)
    ds.xroms.set_grid(xgrid)

    if (not (ds["lat_rho"][0,0] == ds["lat_rho"][0,:]).all() or
       not (ds["lon_rho"][0,0] == ds["lon_rho"][:,0]).all()):
        raise NotImplementedError("The lat/lon grid is not north aligned, so it needs to be interpolated to north aligned, but this is not implemented yet")

    #interpolate u,v,w to rho points
    w_at_rho = xroms.to_grid(ds.w, xgrid, hcoord='rho', scoord='s_rho')
    u_at_rho = xroms.to_grid(ds.u, xgrid, hcoord='rho', scoord='s_rho')
    v_at_rho = xroms.to_grid(ds.v, xgrid, hcoord='rho', scoord='s_rho')

    #rotate them to be north/east facing instead of eta/xi facing
    rotated_u_at_rho, rotated_v_at_rho = xroms.vector.rotate_vectors(u_at_rho, v_at_rho, ds['angle'], xgrid=xgrid)

    #Drop the fields we don't care about
    data_fields_to_save = set(["u", "v", "w", "temp", "salt", "dye_01", "ocean_time", "s_rho", "lon_rho", "lat_rho", "h", "z_rho0"])
    data_fields_to_drop = set(ds.variables.keys()) - data_fields_to_save
    ds = ds.drop_vars(data_fields_to_drop)

    #Update the w,u, and v in the dataset before saving it
    ds["w"] = w_at_rho
    ds["u"] = rotated_u_at_rho
    ds["v"] = rotated_v_at_rho

    #Save regularized dataset
    _, datasets = zip(*ds.groupby("ocean_time"))
    paths = []
    for p in args.model_files:
        paths.append(os.path.join(args.output_path, os.path.basename(p)))
        
    print("Save Regularized NetCDF Model Files")
    xr.save_mfdataset(datasets, paths, format="NETCDF3_64BIT")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output-path", help="The path to output model files")
    parser.add_argument('-m', '--model-files', nargs='+', default=[], help="A list of files to load and regularize")


    args = parser.parse_args()
    main(args)
