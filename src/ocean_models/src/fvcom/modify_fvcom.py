from netCDF4 import Dataset

import sys
import numpy as np
from os import listdir
from os.path import isfile, join, basename

def modifyFVCOM(inputFilename, outputFilename, siglayRange, depth):
    xyScale = 4
    fvcomIn = Dataset(inputFilename, "r")
    fvcomOut = Dataset(outputFilename, "w")

    #Copy the global attributes of the netCDF file
    for attname in fvcomIn.ncattrs():
        setattr(fvcomOut, attname, getattr(fvcomIn, attname))

    #Copy the dimension of the netCDF file
    for dimname, dim in fvcomIn.dimensions.iteritems():
        dimLength = 0

        if dimname == "siglay":
            dimLength = len(siglayRange)
        elif dimname == "siglev":
            dimLength = len(siglayRange) + 1
        else:
            dimLength = len(dim)

        fvcomOut.createDimension(dimname, dimLength)

    #Copy the variables of the netCDF file
    for varname, inVar in fvcomIn.variables.iteritems():

        outVar = fvcomOut.createVariable(varname, inVar.dtype, inVar.dimensions)

        #Copy the variable attributes
        for attname in inVar.ncattrs():
            setattr(outVar, attname, getattr(inVar, attname))

        #Copy the variable data to the new created variable
        if varname == "siglay":
            siglaySize = -1.0 / len(siglayRange)
            siglays = [(i * siglaySize) + (siglaySize / 2) for i in range(0, len(siglayRange))]
            siglayVar = [[s] * inVar.shape[1] for s in siglays]
            outVar[:] = siglayVar

        elif varname == "siglev":
            siglevs = np.linspace(0, -1.0, len(siglayRange) + 1)
            siglevsVar = [[s] * inVar.shape[1] for s in siglevs]
            outVar[:] = siglevsVar

        elif varname == "siglay_center":
            siglaySize = -1.0 / len(siglayRange)
            siglays = [(i * siglaySize) + (siglaySize / 2) for i in range(0, len(siglayRange))]
            siglayVar = [[s] * inVar.shape[1] for s in siglays]
            outVar[:] = siglayVar

        elif varname == "siglev_center":
            siglevs = np.linspace(0, -1.0, len(siglayRange) + 1)
            siglevsVar = [[s] * inVar.shape[1] for s in siglevs]
            outVar[:] = siglevsVar

        elif varname == "x":
            outVar[:] = [x / xyScale for x in inVar[:]]

        elif varname == "y":
            outVar[:] = [x / xyScale for x in inVar[:]]

        elif varname == "xc":
            outVar[:] = [x / xyScale for x in inVar[:]]

        elif varname == "yc":
            outVar[:] = [x / xyScale for x in inVar[:]]

        elif varname == "u":
            siglayLoad = [False] * inVar.shape[1]
            for s in siglayRange:
                siglayLoad[s] = True
            outVar[:] = inVar[:, siglayLoad, :]

        elif varname == "v":
            siglayLoad = [False] * inVar.shape[1]
            for s in siglayRange:
                siglayLoad[s] = True
            outVar[:] = inVar[:, siglayLoad, :]

        elif varname == "omega":
            outVar[:] = np.zeros(outVar.shape)

        elif varname == "ww":
            siglayLoad = [False] * inVar.shape[1]
            for s in siglayRange:
                siglayLoad[s] = True
            outVar[:] = inVar[:, siglayLoad, :]

        elif varname == "temp":
            siglayLoad = [False] * inVar.shape[1]
            for s in siglayRange:
                siglayLoad[s] = True
            outVar[:] = inVar[:, siglayLoad, :]

        elif varname == "salinity":
            siglayLoad = [False] * inVar.shape[1]
            for s in siglayRange:
                siglayLoad[s] = True
            outVar[:] = inVar[:, siglayLoad, :]

        elif varname == "DYE":
            siglayLoad = [False] * inVar.shape[1]
            for s in siglayRange:
                siglayLoad[s] = True
            outVar[:] = inVar[:, siglayLoad, :]

        elif varname == "h_center":
            outVar[:] = np.full(outVar.shape, depth)

        elif varname == "h":
            outVar[:] = np.full(outVar.shape, depth)
        else:
            outVar[:] = inVar[:]

    fvcomIn.close()
    fvcomOut.close()

def main():
    fvcomInput = sys.argv[1]
    fvcomOutput = sys.argv[2]

    siglayRange = range(90, 127, 4)
    depth = 25


    if isfile(fvcomInput) and isfile(fvcomOutput):
        print("From: " + fvcomInput + " To: " + fvcomOutput)
        modifyFVCOM(fvcomInput, fvcomOutput, siglayRange, depth)
    elif isfile(fvcomInput) and not isfile(fvcomOutput):
        print("From: " + fvcomInput + " To: " + join(fvcomOutput, basename(fvcomInput)))
        modifyFVCOM(fvcomInput, join(fvcomOutput, basename(fvcomInput)), siglayRange, depth)
    elif not isfile(fvcomInput) and not isfile(fvcomOutput):
        for f in listdir(fvcomInput):
            if isfile(join(fvcomInput, f)) and f.endswith(".nc"):
                print("From: " + join(fvcomInput, f) + " To: " + join(fvcomOutput, f))
                modifyFVCOM(join(fvcomInput, f), join(fvcomOutput, f), siglayRange, depth)


if __name__== "__main__":
    main()