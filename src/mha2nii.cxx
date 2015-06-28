#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
 
// headers needed for this example
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkImageCast.h>
#include <vtkMetaImageWriter.h>
#include <vtkMetaImageReader.h>
#include <vtkImageMandelbrotSource.h>
#include <vtkImageActor.h>
#include <vtkNIFTIImageReader.h>
#include <vtkNIFTIImageHeader.h>
#include <vtkNIFTIImageWriter.h>



int main(int argc, char* argv[])
{
   // Verify input arguments
   if ( argc != 3 )
   {
      std::cout << "Usage: " << argv[0]
      << " FolderName" << std::endl;
      return EXIT_FAILURE;
   }
 std::string outputFileName = argv[2];
std::string inputFileName = argv[1];

vtkSmartPointer<vtkNIFTIImageReader> reader =
    vtkSmartPointer<vtkNIFTIImageReader>::New();
  reader->SetFileName(inputFileName.c_str());
  reader->Update();

 vtkSmartPointer<vtkMetaImageWriter> writer =
      vtkSmartPointer<vtkMetaImageWriter>::New();
   writer->SetInputConnection(reader->GetOutputPort());
   writer->SetFileName(outputFileName.c_str());
 writer->Write();

 //typedef itk::ImageFileWriter<ImageType> ImageWriterType;
  //ImageWriterType::Pointer imageWriter = ImageWriterType::New();
  //imageWriter->SetInput( reader->GetOutput() );
  //imageWriter->SetFileName( argv[3] );
 return EXIT_SUCCESS;
}

