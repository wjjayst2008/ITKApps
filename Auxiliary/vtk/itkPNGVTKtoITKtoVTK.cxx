/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkPNGVTKtoITKtoVTK.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkCommand.h"
#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRGBPixel.h"
#include "itkImageFileReader.h"

#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkImageShiftScale.h"
#include "vtkPNGReader.h"
#include "vtkImageLuminance.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

/**
 * This will be setup as a callback for a progress event on an ITK
 * filter.
 */
struct ProgressDisplay
{
  ProgressDisplay(itk::ProcessObject* process): m_Process(process) {}
  
  void Display()
    {
    float progress = m_Process->GetProgress()*100.0;
    std::cout << "Progress " << progress << " percent." << std::endl;
    }
  
  itk::ProcessObject::Pointer m_Process;
};


/**
 * This function will connect the given itk::VTKImageExport filter to
 * the given vtkImageImport filter.
 */
template <typename ITK_Exporter, typename VTK_Importer>
void ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer)
{
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());
}

/**
 * This function will connect the given vtkImageExport filter to
 * the given itk::VTKImageImport filter.
 */
template <typename VTK_Exporter, typename ITK_Importer>
void ConnectPipelines(VTK_Exporter* exporter, ITK_Importer importer)
{
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());
}


/**
 * This program implements an example connection between ITK and VTK
 * pipelines.  The combined pipeline flows as follows:
 *
 * vtkPNGReader ==> vtkImageExport ==>
 *   itk::VTKImageImport ==> itk::CurvatureFlowImageFilter ==>
 *   itkVTKImageExport ==> vtkImageImport ==> vtkImageShiftScale ==>
 *   vtkImageActor
 *
 * The resulting vtkImageActor is displayed in a vtkRenderWindow.
 * Whenever the VTK pipeline executes, information is propagated
 * through the ITK pipeline.  If the ITK pipeline is out of date, it
 * will re-execute and cause the VTK pipeline to update properly as
 * well.
 */
int main()
{  

  try {

  //------------------------------------------------------------------------
  // VTK pipeline.
  //------------------------------------------------------------------------
  
  vtkPNGReader * source = vtkPNGReader::New();
  source->SetFileName("ellipses.png");
  source->Update();
  
//  vtkImageLuminance * luminance = vtkImageLuminance::New();
//  luminance->SetInput( source->GetOutput() );

  vtkImageExport* vtkExporter = vtkImageExport::New();
  vtkExporter->SetInput(source->GetOutput());
  
  //------------------------------------------------------------------------
  // VTK to ITK pipeline connection.
  //------------------------------------------------------------------------
  
  typedef itk::Image<unsigned short, 2> ImageType; 
  typedef itk::VTKImageImport<ImageType> ImageImportType;
  ImageImportType::Pointer itkImporter = ImageImportType::New();
  ConnectPipelines(vtkExporter, itkImporter);
  
  //------------------------------------------------------------------------
  // ITK pipeline.
  //------------------------------------------------------------------------

  // Casting from short to double
  typedef itk::Image<double,2> DoubleImageType;
  typedef itk::CastImageFilter< ImageType, DoubleImageType > CastFilterType;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput( itkImporter->GetOutput() );
  
  typedef itk::CurvatureFlowImageFilter<DoubleImageType, DoubleImageType> DenoiserType;
  
  // Create the itk::CurvatureFlowImageFilter and connect it
  // to the itk::RandomImageSource.
  DenoiserType::Pointer denoiser = DenoiserType::New();
  denoiser->SetInput(castFilter->GetOutput());
  denoiser->SetTimeStep(0.15);
  denoiser->SetNumberOfIterations(8);

  // Add a progress observer for the itk::CurvatureFlowImageFilter.
  // This will make it clear when this part of the ITK pipeline
  // executes.
  ProgressDisplay progressDisplay(denoiser);
  itk::SimpleMemberCommand<ProgressDisplay>::Pointer progressEvent =
    itk::SimpleMemberCommand<ProgressDisplay>::New();
  progressEvent->SetCallbackFunction(&progressDisplay,
                                     &ProgressDisplay::Display);
  denoiser->AddObserver(itk::ProgressEvent(), progressEvent);
  
  //------------------------------------------------------------------------
  // ITK to VTK pipeline connection.
  //------------------------------------------------------------------------
  
  typedef itk::VTKImageExport<DoubleImageType> ImageExportType;
  
  // Create the itk::VTKImageExport instance and connect it to the
  // itk::CurvatureFlowImageFilter.
  ImageExportType::Pointer itkExporter = ImageExportType::New();
  itkExporter->SetInput(denoiser->GetOutput());
  
  // Create the vtkImageImport and connect it to the
  // itk::VTKImageExport instance.
  vtkImageImport* vtkImporter = vtkImageImport::New();  
  ConnectPipelines(itkExporter, vtkImporter);
  
  //------------------------------------------------------------------------
  // VTK pipeline.
  //------------------------------------------------------------------------
  
  // Create a vtkImageShiftScale to convert the floating point image
  // to an unsigned char image.  Connect it to the vtkImageImport
  // instance.
  vtkImageShiftScale* shifter = vtkImageShiftScale::New();
  shifter->SetInput(vtkImporter->GetOutput());
  shifter->SetScale(256);
  shifter->SetOutputScalarTypeToUnsignedChar();

  // Create a vtkImageActor to help render the image.  Connect it to
  // the vtkImageShiftScale instance.
  vtkImageActor* actor = vtkImageActor::New();
  actor->SetInput(shifter->GetOutput());
  
  // Create a renderer, render window, and render window interactor to
  // display the results.
  vtkRenderer* renderer = vtkRenderer::New();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
  
  renWin->SetSize(500, 500);
  renWin->AddRenderer(renderer);
  iren->SetRenderWindow(renWin);
  
  // Add the vtkImageActor to the renderer for display.
  renderer->AddActor(actor);
  renderer->SetBackground(0.4392, 0.5020, 0.5647);

  // Bring up the render window and begin interaction.
  renWin->Render();
  iren->Start();
  
  // After the first interaction is quit, modifiy the ITK pipeline and
  // begin interaction again.  The user will see the ITK pipeline
  // re-execute due only to the update request through the VTK
  // pipeline.
  source->SetFileName("ellipses2.png");
  iren->Start();
  
  // VTK does not use smart pointers, so we must clean up its pipeline
  // explicitly.
  iren->Delete();
  renWin->Delete();
  renderer->Delete();
  actor->Delete();
  shifter->Delete();
  vtkImporter->Delete();
  source->Delete();
  vtkExporter->Delete();
  
  
  }
  catch( itk::ExceptionObject & e )
  {
  std::cerr << "Exception catched !! " << e << std::endl;
  }


  // Now attempt instantiation for a color image
  try
    {
    typedef itk::RGBPixel< unsigned char > PixelType;
    typedef itk::Image< PixelType, 2 > ImageType;
    
    typedef itk::ImageFileReader< ImageType > ReaderType;

    ReaderType::Pointer reader  = ReaderType::New();
    reader->SetFileName("ellipses2.png");
    reader->Update();

    typedef itk::VTKImageExport< ImageType > ExportFilterType;
    ExportFilterType::Pointer itkExporter = ExportFilterType::New();

    itkExporter->SetInput( reader->GetOutput() );

    // Create the vtkImageImport and connect it to the
    // itk::VTKImageExport instance.
    vtkImageImport* vtkImporter = vtkImageImport::New();  
    ConnectPipelines(itkExporter, vtkImporter);
    
    //------------------------------------------------------------------------
    // VTK pipeline.
    //------------------------------------------------------------------------
    
    // Create a vtkImageShiftScale to convert the floating point image
    // to an unsigned char image.  Connect it to the vtkImageImport
    // instance.
    vtkImageShiftScale* shifter = vtkImageShiftScale::New();
    shifter->SetInput(vtkImporter->GetOutput());
    shifter->SetScale(256);
    shifter->SetOutputScalarTypeToUnsignedChar();


    // Create a vtkImageActor to help render the image.  Connect it to
    // the vtkImageShiftScale instance.
    vtkImageActor* actor = vtkImageActor::New();
    actor->SetInput(shifter->GetOutput());
    
    // Create a renderer, render window, and render window interactor to
    // display the results.
    vtkRenderer* renderer = vtkRenderer::New();
    vtkRenderWindow* renWin = vtkRenderWindow::New();
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
    
    renWin->SetSize(500, 500);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);
    
    // Add the vtkImageActor to the renderer for display.
    renderer->AddActor(actor);
    renderer->SetBackground(0.4392, 0.5020, 0.5647);

    // Bring up the render window and begin interaction.
    renWin->Render();
    iren->Start();

    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception catched !! " << e << std::endl;
    }


  
  return 0;
}
