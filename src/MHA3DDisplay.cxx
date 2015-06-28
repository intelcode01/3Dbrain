#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkMetaImageReader.h>
// headers needed for this example
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkVolume.h>
#include <vtkVolumeMapper.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkSphere.h>
#include <vtkSampleFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkCamera.h>
#include <vtkImageShiftScale.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkXMLImageDataReader.h>

// needed to easily convert int to std::string
#include <sstream>


int main(int argc, char* argv[])
{
   // Verify input arguments
   if ( argc != 2 )
   {
      std::cout << "Usage: " << argv[0]
      << " FolderName" << std::endl;
      return EXIT_FAILURE;
   }

vtkSmartPointer<vtkImageData> imageData =
    vtkSmartPointer<vtkImageData>::New();
 
   std::string folder = argv[1];
   //std::string folder = "C:\\VTK\\vtkdata-5.8.0\\Data\\DicomTestImages";
 
   // Read all the DICOM files in the specified directory.
  // vtkSmartPointer<vtkDICOMImageReader> reader =
    //  vtkSmartPointer<vtkDICOMImageReader>::New();
   //reader->SetDirectoryName(folder.c_str());
   //reader->Update();

 vtkSmartPointer<vtkMetaImageReader> reader =
    vtkSmartPointer<vtkMetaImageReader>::New();
  reader->SetFileName(folder.c_str());
  reader->Update();


 imageData->ShallowCopy(reader->GetOutput());


 vtkSmartPointer<vtkRenderWindow> renWin = 
    vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderer> ren1 = 
    vtkSmartPointer<vtkRenderer>::New();
  ren1->SetBackground(0.0,0.0,0.0);
 
  renWin->AddRenderer(ren1);
 
  renWin->SetSize(301,300); // intentional odd and NPOT  width/height
 
  vtkSmartPointer<vtkRenderWindowInteractor> iren = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);
 
  renWin->Render(); // make sure we have an OpenGL context.
 
  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = 
    vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper->SetBlendModeToComposite(); // composite first
#if VTK_MAJOR_VERSION <= 5
  volumeMapper->SetInputConnection(imageData->GetProducerPort());
#else
  volumeMapper->SetInputData(imageData);
#endif  
  vtkSmartPointer<vtkVolumeProperty> volumeProperty = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->ShadeOff();
  volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
 
  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();
  //compositeOpacity->AddPoint(1.0,0.0);
  //compositeOpacity->AddPoint(5.0,0.1);
  //compositeOpacity->AddPoint(100.0,1.0);
  //compositeOpacity->AddPoint(0, 0);
  //compositeOpacity->AddPoint(10.0, 0.01);
  //compositeOpacity->AddPoint(100.0, 0.01);

//compositeOpacity->AddPoint(0,    0.00);
 //compositeOpacity->AddPoint(500,  0.15);
 //compositeOpacity->AddPoint(1000, 0.15);
 //compositeOpacity->AddPoint(1150, 0.85);

//Use for showing RGB Opacity
//compositeOpacity->AddPoint(0, 0.0);
//compositeOpacity->AddPoint(5.0, 0.0);
//compositeOpacity->AddPoint(30.0, 0.05);
//compositeOpacity->AddPoint(31.0, 0.0);
//compositeOpacity->AddPoint(90.0, 0.0);
//compositeOpacity->AddPoint(100.0, 0.3);
//compositeOpacity->AddPoint(110.0, 0.0);
//compositeOpacity->AddPoint(190.0, 0.0);
//compositeOpacity->AddPoint(210.0, 0.0);
//compositeOpacity->AddPoint(245.0, 0.0);
//compositeOpacity->AddPoint(255.0, 0.5);

//Use for Highlighting muscle

 //compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0 );
 //compositeOpacity->AddPoint(-155, 0, 0.5, 0.92 );
 //compositeOpacity->AddPoint(217, .68, 0.33, 0.45 );
//compositeOpacity->AddPoint(420,.83, 0.5, 0.0);
//compositeOpacity->AddPoint(3071, .80, 0.5, 0.0);

//Use for Bone

//compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0 );
 //compositeOpacity->AddPoint(-16, 0, .49, .61 );
//compositeOpacity->AddPoint(641, .72, .5, 0.0 );
 //compositeOpacity->AddPoint(3071, .71, 0.5, 0.0);


//Use for Skin
//compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0 );
//compositeOpacity->AddPoint(-1000, 0, 0.5, 0.0 );
//compositeOpacity->AddPoint(-500, 1.0, 0.33, 0.45 );
//compositeOpacity->AddPoint(3071, 1.0, 0.5, 0.0);

//Use for Brain
//compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0 );
//compositeOpacity->AddPoint(-10, 0, 0.4, 0.5 );
//compositeOpacity->AddPoint(800, 0.8, 0.5, 0.0 );
//compositeOpacity->AddPoint(3071, 0.5, 0.5, 0.0);

compositeOpacity->AddPoint(20, 0.0);
compositeOpacity->AddPoint(255, 0.2);


  volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

 //vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
  //vtkSmartPointer<vtkPiecewiseFunction>::New();
//volumeGradientOpacity->AddPoint(0,   0.0);
//volumeGradientOpacity->AddPoint(90,  0.5);
  // volumeGradientOpacity->AddPoint(100, 1.0);
//volumeProperty->SetGradientOpacity(volumeGradientOpacity);
 
  vtkSmartPointer<vtkColorTransferFunction> color = 
    vtkSmartPointer<vtkColorTransferFunction>::New();
 //color->AddRGBPoint(80.0,  0.0, 0.0, 0.0);
  //color->AddRGBPoint(120.0, 0.0, 0.0, 1.0);
  //color->AddRGBPoint(160.0, 1.0, 0.0, 0.0);
  //color->AddRGBPoint(200.0, 0.0, 1.0, 0.0);
 //color->AddRGBPoint(255.0, 0.0, 1.0, 1.0);
 //color->AddRGBPoint(0.0  ,0.0,0.0,1.0);
  //color->AddRGBPoint(40.0  ,1.0,0.0,0.0);
  //color->AddRGBPoint(255.0,1.0,1.0,1.0);
 //color->AddRGBPoint(0, 0,0,0);
 //color->AddRGBPoint(5.0, 0,0,0);
// color->AddRGBPoint(5.0, 1,0,0);

//Use for Muscle Highlighitng
          // color->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
        //color->AddRGBPoint( -155, .55, .25, .15, 0.5, .92 );
       //color->AddRGBPoint( 217, .88, .60, .29, 0.33, 0.45 );
       //color->AddRGBPoint( 420, 1, .94, .95, 0.5, 0.0 );
      // color->AddRGBPoint( 3071, .83, .66, 1, 0.5, 0.0 );

 //color->AddRGBPoint(0,    0.0, 0.0, 0.0);
 // color->AddRGBPoint(500,  1.0, 0.5, 0.3);
  //color->AddRGBPoint(1000, 1.0, 0.5, 0.3);
 // color->AddRGBPoint(1150, 1.0, 1.0, 0.9);

//Use for Bone
//color->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
  //color->AddRGBPoint( -16, 0.73, 0.25, 0.30, 0.49, .61 );
    //    color->AddRGBPoint( 641, .90, .82, .56, .5, 0.0 );
      //  color->AddRGBPoint( 3071, 1, 1, 1, .5, 0.0 );

//Use for Skin
//color->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
  //  color->AddRGBPoint( -1000, .62, .36, .18, 0.5, 0.0 );
   // color->AddRGBPoint( -500, .88, .60, .29, 0.33, 0.45 );
   // color->AddRGBPoint( 3071, .83, .66, 1, 0.5, 0.0 );

//Use for Brain
//color->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
  //  color->AddRGBPoint( -1000, .8, .20, .40, 0.5, 0.7 );
   // color->AddRGBPoint( 800, .90, .80, .29, 0.6, 0.0 );
   // color->AddRGBPoint( 3071, 1, 1, 1, 0.5, 0.0 );

color->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
color->AddRGBPoint(64.0, 1.0, 0.0, 0.0);
color->AddRGBPoint(128.0, 0.0, 0.0, 1.0);
color->AddRGBPoint(192.0, 0.0, 1.0, 0.0);
color->AddRGBPoint(255.0, 0.0, 0.2, 0.0);
//Setting Opacity and Color Functions
  volumeProperty->SetColor(color);
 
  vtkSmartPointer<vtkVolume> volume = 
    vtkSmartPointer<vtkVolume>::New();
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);
  ren1->AddViewProp(volume);
  ren1->ResetCamera();
 
  // Render composite. In default mode. For coverage.
  renWin->Render();
 
  // 3D texture mode. For coverage.
  volumeMapper->SetRequestedRenderModeToDefault();
  renWin->Render();
 
  // Software mode, for coverage. It also makes sure we will get the same
  // regression image on all platforms.
  volumeMapper->SetRequestedRenderModeToDefault();
  renWin->Render();
 //Initiation
  iren->Start();
 
  return EXIT_SUCCESS;
}



