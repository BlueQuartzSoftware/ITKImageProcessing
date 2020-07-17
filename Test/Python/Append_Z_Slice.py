# Based on Image Initial Visualization example
# Tests the Append Z Slice filter
# These are the simpl_py python modules

import simpl
import simpl_helpers as sc
import simpl_test_dirs as sd
import itkimageprocessing
import itkimageprocessingpy
import samplingpy

def append_z_slice():
    # Create Data Container Array
    dca = simpl.DataContainerArray()

    # Register the ImageIO Factories
    image_writer = itkimageprocessing.ITKImageWriter()
    image_writer.registerImageIOFactories()

    # Import Image Stack [ITK]
    file_list_info = simpl.FileListInfo(2, 0, 11, 174, 1, sd.GetDataDirectory() + '/Data/Image',
                                        'slice_', '', 'tif')
    err = itkimageprocessingpy.itk_import_image_stack(dca, 'RoboMet.3D Image Stack', 'Optical Data',
                                                      simpl.FloatVec3Type(0, 0, 0), simpl.FloatVec3Type(1, 1, 1),
                                                      '', file_list_info, 164, 'ImageData')
    assert err == 0, f'ITKImportImageStack ErrorCondition {err}'

    # Append Image Geometry Z Slice #1
    err = samplingpy.append_image_geometry_z_slice(dca,
                                                   simpl.DataArrayPath('RoboMet.3D Image Stack',
                                                                       'Optical Data', ''),
                                                   simpl.DataArrayPath('RoboMet.3D Image Stack',
                                                                       'Optical Data', ''), False)
    assert err == 0, f'AppendImageGeometryZSlice #1 ErrorCondition {err}'

    # Append Image Geometry Z Slice #2
    err = samplingpy.append_image_geometry_z_slice(dca,
                                                   simpl.DataArrayPath('RoboMet.3D Image Stack',
                                                                       'Optical Data', ''),
                                                   simpl.DataArrayPath('RoboMet.3D Image Stack',
                                                                       'Optical Data', ''), False)
    assert err == 0, f'AppendImageGeometryZSlice #2 ErrorCondition {err}'

    # Write to DREAM3D file
    err = sc.WriteDREAM3DFile(sd.GetBuildDirectory() +
                              '/Data/Output/CoreFilterTests/' + 
                              'AppendImageGeometryZSlice.dream3d',
                              dca)
    assert err == 0, f'WriteDREAM3DFile ErrorCondition {err}'

'''
Main entry point for python script
'''
if __name__ == '__main__':
    append_z_slice()
