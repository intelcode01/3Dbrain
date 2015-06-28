#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNIFTIImageReader.h>
#include <vtkNIFTIImageHeader.h>
#include <vtkNIFTIImageWriter.h>


 
int main(int argc, char *argv[])
{
  std::string inputFilename;
 
  if(argc < 2)
  {
    std::cerr << "Required arguments: image.mha" << std::endl;
    return EXIT_FAILURE;
  }
 
  inputFilename = argv[1];
 
 // vtkSmartPointer<vtkMetaImageReader> reader =
   // vtkSmartPointer<vtkMetaImageReader>::New();
  //reader->SetFileName(inputFilename.c_str());
 // reader->Update();

vtkSmartPointer<vtkNIFTIImageReader> reader =
    vtkSmartPointer<vtkNIFTIImageReader>::New();
  reader->SetFileName(argv[1]);
  reader->Update();
 
  // Visualize
  vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
  actor->GetMapper()->SetInputConnection(reader->GetOutputPort());
 //Renderer
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  renderer->ResetCamera();
 //Adding Render
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
 //Render Window
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
 //Setting Rendering Window
  renderWindowInteractor->SetInteractorStyle(style);
 
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->Initialize();
 //Intiating
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}
