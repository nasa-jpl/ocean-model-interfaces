import argparse
import os
import xroms
import xarray as xr

def process_single_file(input_file, output_file):
    """Process a single ROMS file to avoid memory issues"""
    print(f"\nProcessing: {os.path.basename(input_file)}")

    # Load single file
    ds = xr.open_dataset(input_file)
    ds, xgrid = xroms.roms_dataset(ds, include_cell_volume=True, include_Z0=True)
    ds.xroms.set_grid(xgrid)

    # Interpolate u,v,w to rho points
    print("  Interpolating to rho grid...")
    w_at_rho = xroms.to_grid(ds.w, xgrid, hcoord='rho', scoord='s_rho')
    u_at_rho = xroms.to_grid(ds.u, xgrid, hcoord='rho', scoord='s_rho')
    v_at_rho = xroms.to_grid(ds.v, xgrid, hcoord='rho', scoord='s_rho')

    # Rotate velocity vectors
    print("  Rotating vectors...")
    rotated_u_at_rho, rotated_v_at_rho = xroms.vector.rotate_vectors(u_at_rho, v_at_rho, ds['angle'], xgrid=xgrid)

    # Drop unnecessary fields
    data_fields_to_save = set(["u", "v", "w", "temp", "salt", "dye_01", "ocean_time", "s_rho", "lon_rho", "lat_rho", "h", "z_rho0"])
    data_fields_to_drop = set(ds.variables.keys()) - data_fields_to_save
    ds = ds.drop_vars(data_fields_to_drop)

    # Update the dataset with interpolated/rotated values
    ds["w"] = w_at_rho
    ds["u"] = rotated_u_at_rho
    ds["v"] = rotated_v_at_rho

    # Compute and save
    print(f"  Computing and saving to {os.path.basename(output_file)}...")
    ds = ds.compute()
    ds.to_netcdf(output_file, format="NETCDF3_64BIT")

    print(f"  ✓ Saved: {os.path.basename(output_file)}")
    ds.close()

def main(args):
    # Create output directory if it doesn't exist
    os.makedirs(args.output_path, exist_ok=True)
    print(f"Output directory: {args.output_path}")
    print(f"Processing {len(args.model_files)} files...")

    # Check grid alignment with first file
    print("\nChecking grid alignment (first file)...")
    ds_check = xr.open_dataset(args.model_files[0])
    if (not (ds_check["lat_rho"][0,0] == ds_check["lat_rho"][0,:]).all() or
       not (ds_check["lon_rho"][0,0] == ds_check["lon_rho"][:,0]).all()):
        ds_check.close()
        raise NotImplementedError("The lat/lon grid is not north aligned, so it needs to be interpolated to north aligned, but this is not implemented yet")
    ds_check.close()
    print("✓ Grid is north-aligned")

    # Process each file individually to avoid memory issues
    for i, input_file in enumerate(args.model_files, 1):
        output_file = os.path.join(args.output_path, os.path.basename(input_file))
        print(f"\n[{i}/{len(args.model_files)}]", end=" ")

        try:
            process_single_file(input_file, output_file)
        except Exception as e:
            print(f"  ✗ Error processing {os.path.basename(input_file)}: {e}")
            raise

    print(f"\n{'='*60}")
    print(f"Done! Processed {len(args.model_files)} files successfully.")
    print(f"Output location: {args.output_path}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output-path", help="The path to output model files")
    parser.add_argument('-m', '--model-files', nargs='+', default=[], help="A list of files to load and regularize")


    args = parser.parse_args()
    main(args)
