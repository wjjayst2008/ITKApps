/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkPolyDataToITKMesh.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Disable warning for long symbol names in this file only
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif



//
//
//  This program illustrates how to convert 
//  a vtkPolyData structure into an itk::Mesh.
//  
//  The example is tailored for the case in which the vtkPolyData
//  only containes triangle strips.
//
//
#include <iostream>



//
// ITK Headers
// 
#include "itkMesh.h"
#include "itkLineCell.h"
#include "itkTriangleCell.h"



//
// VTK headers
//
#include "vtkPolyDataReader.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"




int main( int argc, char * argv [] )
{

  if( argc < 2 )
    {
    std::cerr << "Missing arguments" << std::endl;
    std::cerr << "Usage: vtkPolyDataToITKMesh   vtkPolyDataInputFile" << std::endl;
    return -1;
    }

  vtkPolyDataReader * reader = vtkPolyDataReader::New();

  reader->SetFileName( argv[1] );
  reader->Update();

  vtkPolyData * polyData = reader->GetOutput();


// This define is needed to deal with double/float changes in VTK
#ifndef vtkFloatingPointType
#define vtkFloatingPointType float
#endif



  const unsigned int PointDimension   = 3;
  const unsigned int MaxCellDimension = 2;
  
  typedef itk::DefaultStaticMeshTraits< 
                      vtkFloatingPointType, 
                      PointDimension,
                      MaxCellDimension, 
                      vtkFloatingPointType, 
                      vtkFloatingPointType  >       MeshTraits;

  
  typedef itk::Mesh<
                      vtkFloatingPointType, 
                      PointDimension, 
                      MeshTraits              >     MeshType;


  MeshType::Pointer  mesh = MeshType::New();


  //
  // Transfer the points from the vtkPolyData into the itk::Mesh
  //
  const unsigned int numberOfPoints = polyData->GetNumberOfPoints();
  
  vtkPoints * vtkpoints = polyData->GetPoints();

  mesh->GetPoints()->Reserve( numberOfPoints );
  
  for(int p =0; p < numberOfPoints; p++)
    {

    vtkFloatingPointType * apoint = vtkpoints->GetPoint( p );
    
    mesh->SetPoint( p, MeshType::PointType( apoint ));
    
    }

  
  //
  // Transfer the cells from the vtkPolyData into the itk::Mesh
  //
  vtkCellArray * triangleStrips = polyData->GetStrips();

  const unsigned int numberOfCells = triangleStrips->GetNumberOfCells();

  

  vtkIdType  * cellPoints;
  vtkIdType    numberOfCellPoints;

  //
  // First count the total number of triangles from all the triangle strips.
  //
  unsigned int numberOfTriangles = 0;

  triangleStrips->InitTraversal();

  while( triangleStrips->GetNextCell( numberOfCellPoints, cellPoints ) )
    {
    numberOfTriangles += numberOfCellPoints-2;
    }



  //
  // Reserve memory in the itk::Mesh for all those triangles
  //
  mesh->GetCells()->Reserve( numberOfTriangles );


  
  // 
  // Copy the triangles from vtkPolyData into the itk::Mesh
  //
  //

  typedef MeshType::CellType   CellType;

  typedef itk::TriangleCell< CellType > TriangleCellType;

  int cellId = 0;

  triangleStrips->InitTraversal();
  while( triangleStrips->GetNextCell( numberOfCellPoints, cellPoints ) )
    {
    
    unsigned int numberOfTrianglesInStrip = numberOfCellPoints - 2;

    for( unsigned int t=0; t < numberOfTrianglesInStrip; t++ )
      {
      MeshType::CellAutoPointer c;
      TriangleCellType * t = new TriangleCellType;
      t->SetPointIds( (unsigned long*)cellPoints );
      c.TakeOwnership( t );
      mesh->SetCell( cellId, c );
      cellId++;
      }

    
    }



  std::cout << "Mesh  " << std::endl;
  std::cout << "Number of Points =   " << mesh->GetNumberOfPoints() << std::endl;
  std::cout << "Number of Cells  =   " << mesh->GetNumberOfCells()  << std::endl;



  // Release vtk objects
  //
  reader->Delete();




  return 0;

}




