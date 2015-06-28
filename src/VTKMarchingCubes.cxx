#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkMarchingCubes.h>
#include <vtkVoxelModeller.h>
#include <vtkSphereSource.h>
#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>
 
#include <vtkSTLWriter.h>
#include <vtkSTLReader.h>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkMetaImageReader.h>
 
int main(int argc, char *argv[])
{
  vtkSmartPointer<vtkImageData> volume =
    vtkSmartPointer<vtkImageData>::New();
  double isoValue;
  
std::string folder = argv[1];
std::string filename = argv[3];

vtkSmartPointer<vtkMetaImageReader> reader =
    vtkSmartPointer<vtkMetaImageReader>::New();
  reader->SetFileName(folder.c_str());
  reader->Update();    
   

    volume->DeepCopy(reader->GetOutput());
    isoValue = atof(argv[2]);
    
 
  vtkSmartPointer<vtkMarchingCubes> surface = 
    vtkSmartPointer<vtkMarchingCubes>::New();
 
#if VTK_MAJOR_VERSION <= 5
  surface->SetInput(volume);
#else
  surface->SetInputData(volume);
#endif
  surface->ComputeNormalsOn();
  surface->SetValue(0, isoValue);

vtkSmartPointer<vtkSTLWriter> stlWriter =
    vtkSmartPointer<vtkSTLWriter>::New();
  stlWriter->SetFileName(filename.c_str());
  stlWriter->SetInputConnection(surface->GetOutputPort());
  stlWriter->Write();


 
  vtkSmartPointer<vtkRenderer> renderer = 
    vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground(.1, .2, .3);
 
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> interactor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);
 
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(surface->GetOutputPort());
  mapper->ScalarVisibilityOff();
 
  vtkSmartPointer<vtkActor> actor = 
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
 
  renderer->AddActor(actor);
 
  renderWindow->Render();
  interactor->Start();
  return EXIT_SUCCESS;
}
