#include "time.h"
#include "vnl/vnl_math.h"

#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkMaskImageFilter.h"

#include "itkStripTsImageFilter.h"

int main( int argc, char* argv[] )
{
  if( argc < 6 )
  {
    std::cerr << "Usage: " << argv[0]
              << " patientImageFile atlasImageFile atlasMaskFile"
              << " outputMask outputMaskedPatientImage"\
              << std::endl;
    return EXIT_FAILURE;

  }

  double startTime = time(0);

// We define the pixel type and dimension of the image to be read. In this 
// particular case, the dimensionality of the image is 3, and we assume a
// \code{signed short} pixel type that is commonly used for X-Rays CT scanners.
//
// The image orientation information contained in the direction cosines
// of the DICOM header are read in and passed correctly down the image processing
// pipeline.

  typedef signed short    PixelType;
  const unsigned int      Dimension = 3;
  typedef itk::Image< PixelType, Dimension >         ImageType;

// We use the image type for instantiating the type of the series reader and
// for constructing one object of its type.

  typedef itk::ImageSeriesReader< ImageType >        ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

// A GDCMImageIO object is created and connected to the reader. This object is
// the one that is aware of the internal intricacies of the DICOM format.

  typedef itk::GDCMImageIO       ImageIOType;
  ImageIOType::Pointer dicomIO = ImageIOType::New();
  reader->SetImageIO( dicomIO );



  typedef itk::GDCMSeriesFileNames NamesGeneratorType;
  NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
  nameGenerator->SetUseSeriesDetails( true );
  nameGenerator->AddSeriesRestriction("0008|0021" );
  nameGenerator->SetDirectory( argv[1] );
// Software Guide : EndCodeSnippet
  try
    {
    std::cout << std::endl << "The directory: " << std::endl;
    std::cout << std::endl << argv[1] << std::endl << std::endl;
    std::cout << "Contains the following DICOM Series: ";
    std::cout << std::endl << std::endl;

// The GDCMSeriesFileNames object first identifies the list of DICOM series
// that are present in the given directory. We receive that list in a reference
// to a container of strings and then we can do things like printing out all
// the series identifiers that the generator had found. Since the process of
// finding the series identifiers can potentially throw exceptions, it is
// wise to put this code inside a try/catch block.

    typedef std::vector< std::string >    SeriesIdContainer;
    const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
    SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
    SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
    while( seriesItr != seriesEnd )
      {
      std::cout << seriesItr->c_str() << std::endl;
      ++seriesItr;
      }

// Given that it is common to find multiple DICOM series in the same directory,
// we must tell the GDCM classes what specific series do we want to read. In
// this example we do this by checking first if the user has provided a series
// identifier in the command line arguments. If no series identifier has been
// passed, then we simply use the first series found during the exploration of
// the directory.

    std::string seriesIdentifier;
   
      seriesIdentifier = seriesUID.begin()->c_str();

    std::cout << std::endl << std::endl;
    std::cout << "Now reading series: " << std::endl << std::endl;
    std::cout << seriesIdentifier << std::endl;
    std::cout << std::endl << std::endl;

// We pass the series identifier to the name generator and ask for all the
// filenames associated to that series. This list is returned in a container of
// strings by the \code{GetFileNames()} method.

    typedef std::vector< std::string >   FileNamesContainer;
    FileNamesContainer fileNames;
    fileNames = nameGenerator->GetFileNames( seriesIdentifier );





    reader->SetFileNames( fileNames );

// Finally we can trigger the reading process by invoking the 
// method in the reader. This call as usual is placed inside a \code{try/catch}
// block.

    try
      {
      reader->Update();
      }
    catch (itk::ExceptionObject &ex)
      {
      std::cout << ex << std::endl;
      return EXIT_FAILURE;
      }
  }
  catch (itk::ExceptionObject &ex)
    {
    std::cout << ex << std::endl;
    return EXIT_FAILURE;
    }


  std::string atlasImageFilename = argv[2];
  std::string atlasMaskFilename = argv[3];

 
  typedef itk::Image<short, 3> AtlasImageType;
  typedef itk::Image<unsigned char, 3> AtlasLabelType;


  // image reading
  
  typedef itk::ImageFileReader<AtlasImageType> AtlasReaderType;
  typedef itk::ImageFileReader<AtlasLabelType> LabelReaderType;
 
  AtlasReaderType::Pointer atlasReader = AtlasReaderType::New();
  LabelReaderType::Pointer labelReader = LabelReaderType::New();


  atlasReader->SetFileName( atlasImageFilename );
  labelReader->SetFileName( atlasMaskFilename );

  try
    {
   
    atlasReader->Update();
    labelReader->Update();
    }
  catch ( itk::ExceptionObject &exception )
    {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exception << std::endl;

    return EXIT_FAILURE;
    }

  // perform skull-stripping using stripTsImageFilter
  std::cout << std::endl << "Performing skull-stripping" << std::endl;

  // set up skull-stripping filter
  typedef itk::StripTsImageFilter<ImageType, AtlasImageType, AtlasLabelType> StripTsFilterType;
  StripTsFilterType::Pointer stripTsFilter = StripTsFilterType::New();

  // set the required inputs for the stripTsImageFilter
  stripTsFilter->SetInput( reader->GetOutput() );
  stripTsFilter->SetAtlasImage( atlasReader->GetOutput() );
  stripTsFilter->SetAtlasBrainMask( labelReader->GetOutput() );

  try
    {
    stripTsFilter->Update();
    }
  catch ( itk::ExceptionObject &exception )
    {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exception << std::endl;

    return EXIT_FAILURE;
    }


  // mask the user image using the output generated from the stripTsImageFilter as mask
  typedef itk::MaskImageFilter<ImageType, AtlasLabelType, ImageType> MaskFilterType;
  MaskFilterType::Pointer maskFilter = MaskFilterType::New();

  maskFilter->SetInput1( reader->GetOutput() );
  maskFilter->SetInput2( stripTsFilter->GetOutput() );

  try
    {
    maskFilter->Update();
    }
  catch ( itk::ExceptionObject &exception )
    {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exception << std::endl;

    return EXIT_FAILURE;
    }


  // write mask and masked user image
  typedef itk::ImageFileWriter<AtlasLabelType> MaskWriterType;
  MaskWriterType::Pointer maskWriter = MaskWriterType::New();
  maskWriter->SetInput( stripTsFilter->GetOutput() );
  maskWriter->SetFileName( argv[4] );

  typedef itk::ImageFileWriter<ImageType> ImageWriterType;
  ImageWriterType::Pointer imageWriter = ImageWriterType::New();
  imageWriter->SetInput( maskFilter->GetOutput() );
  imageWriter->SetFileName( argv[5] );

  try
    {
    maskWriter->Update();
    imageWriter->Update();
    }
  catch ( itk::ExceptionObject &exception )
    {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exception << std::endl;

    return EXIT_FAILURE;
    }


  double endTime = time(0);
  std::cout << "Total computation time: " << endTime-startTime << "seconds " << std::endl;


  return EXIT_SUCCESS;
}
