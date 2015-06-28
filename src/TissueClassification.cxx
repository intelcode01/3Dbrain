#include "itkConnectedThresholdImageFilter.h"

#include "itkImage.h"
#include "itkCastImageFilter.h"

//  Noise present in the image can reduce the capacity of this filter to grow
//  large regions. When faced with noisy images, it is usually convenient to
//  pre-process the image by using an edge-preserving smoothing filter. Any of
//  the filters discussed in Section~\ref{sec:EdgePreservingSmoothingFilters}
//  could be used to this end. In this particular example we use the
//  \doxygen{CurvatureFlowImageFilter}, so we need to include its header
//  file.


#include "itkCurvatureFlowImageFilter.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
int main( int argc, char *argv[])
{
  if( argc < 8 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage  outputImage seedX seedY lowerThreshold upperThreshold" << std::endl;
    return 1;
    }
  
  //  We declare the image type based on a particular pixel type and
  //  dimension. In this case the \code{float} type is used for the pixels
  //  due to the requirements of the smoothing filter.
 
  typedef   float           InternalPixelType;
  const     unsigned int    Dimension = 3;
  typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
  // Software Guide : EndCodeSnippet
  typedef unsigned char                            OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  typedef itk::CastImageFilter< InternalImageType, OutputImageType >
                                                   CastingFilterType;
  CastingFilterType::Pointer caster = CastingFilterType::New();
  // We instantiate reader and writer types
  //
  typedef  itk::ImageFileReader< InternalImageType > ReaderType;
  typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );
 
  //  The smoothing filter is instantiated using the image type as
  //  a template parameter.
  
  typedef itk::CurvatureFlowImageFilter< InternalImageType, InternalImageType >
    CurvatureFlowImageFilterType;
  
  //  Then the filter is created by invoking the \code{New()} method and
  //  assigning the result to a \doxygen{SmartPointer}.
 
  CurvatureFlowImageFilterType::Pointer smoothing =
                         CurvatureFlowImageFilterType::New();
  
  //  We now declare the type of the region growing filter. In this case it is
  //  the ConnectedThresholdImageFilter.
 
  typedef itk::ConnectedThresholdImageFilter< InternalImageType,
                                    InternalImageType > ConnectedFilterType;
 
  //  Then we construct one filter of this class using the \code{New()}
  //  method.
 
  ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
 
  //  Now it is time to connect a simple, linear pipeline. A file reader is
  //  added at the beginning of the pipeline and a cast filter and writer
  //  are added at the end. The cast filter is required to convert
  //  \code{float} pixel types to integer types since only a few image file
  //  formats support \code{float} types.
 
  smoothing->SetInput( reader->GetOutput() );
  connectedThreshold->SetInput( smoothing->GetOutput() );
  caster->SetInput( connectedThreshold->GetOutput() );
  writer->SetInput( caster->GetOutput() );
  
  //  The CurvatureFlowImageFilter requires a couple of parameters. 
  //  The following are typical values for $2D$ images. However
  //  they may have to be adjusted depending on the amount of noise present in
  //  the input image.
  
  smoothing->SetNumberOfIterations( 5 );
  smoothing->SetTimeStep( 0.125 );
  
  //  The ConnectedThresholdImageFilter has two main parameters to be
  //  defined. They are the lower and upper thresholds of the interval in
  //  which intensity values should fall in order to be included in the
  //  region. Setting these two values too close will not allow enough
  //  flexibility for the region to grow. Setting them too far apart will
  //  result in a region that engulfs the image.
  //
  //  \index{itk::ConnectedThresholdImageFilter!SetUpper()}
  //  \index{itk::ConnectedThresholdImageFilter!SetLower()}
 
  const InternalPixelType lowerThreshold = atof( argv[6] );
  const InternalPixelType upperThreshold = atof( argv[7] );
 
  connectedThreshold->SetLower(  lowerThreshold  );
  connectedThreshold->SetUpper(  upperThreshold  );
  
  //  The output of this filter is a binary image with zero-value pixels
  //  everywhere except on the extracted region. The intensity value set
  //  inside the region is selected with the method \code{SetReplaceValue()}.
  //
  //  \index{itk::ConnectedThresholdImageFilter!SetReplaceValue()}
  
  connectedThreshold->SetReplaceValue( 255 );
  
  //  The initialization of the algorithm requires the user to provide a seed
  //  point. It is convenient to select this point to be placed in a
  //  \emph{typical} region of the anatomical structure to be segmented. The
  //  seed is passed in the form of a \doxygen{Index} to the \code{SetSeed()}
  //  method.
  //
  //  \index{itk::ConnectedThresholdImageFilter!SetSeed()}
  
  InternalImageType::IndexType  index;
  index[0] = atoi( argv[3] );
  index[1] = atoi( argv[4] );
  index[2] = atoi( argv[5] );
 
  connectedThreshold->SetSeed( index );
  
  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  It is usually wise to put update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.
 
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }
  
  
  
  //  White matter & $(60,116)$ & 150 & 180 & Second from left in Figure \ref{fig:ConnectedThresholdOutput} \\ \hline
  //  Ventricle    & $(81,112)$ & 210 & 250 & Third  from left in Figure \ref{fig:ConnectedThresholdOutput} \\ \hline
  //  Gray matter  & $(107,69)$ & 180 & 210 & Fourth from left in Figure \ref{fig:ConnectedThresholdOutput} \\ \hline
 
  
  return 0;
}
