/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    IRISImageData.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __IRISImageData_h_
#define __IRISImageData_h_

#include "IRISTypes.h"
#include "IRISException.h"
#include "LabelImageWrapper.h"
#include "GreyImageWrapper.h"
#include "ColorLabel.h"

/**
 * \class IRISImageData
 * \brief This class encapsulates the image data used by 
 * the IRIS component of SnAP.  
 *
 * This data consists of a grey image [gi] and a segmentation image [si].
 * The following rules must be satisfied by this class:
 *  + exists(si) ==> exists(gi)
 *  + if exists(si) then size(si) == size(gi)
 */
class IRISImageData {
public:
  typedef GreyImageWrapper GreyWrapperType;
  typedef LabelImageWrapper LabelWrapperType;

  IRISImageData();
  virtual ~IRISImageData();

  /**
   * Returns a reference to color label at specified index.
   */
  ColorLabel &GetColorLabel(unsigned int index) {
    assert(index < MAX_COLOR_LABELS);
    return m_ColorLabels[index];
  }

  /**
   * Returns a reference to color label at specified index.
   */
  ColorLabel GetColorLabel(unsigned int index) const {
    assert(index < MAX_COLOR_LABELS);
    return m_ColorLabels[index];
  }

  /**
   * Returns the number of allocated color labels
   */
  unsigned int GetColorLabelCount() const {
    return m_ColorLabelCount;
  }

  /**
   * Read color label table from disk.
   */
  void ReadASCIIColorLabels(const char* filename);

  /**
   * Write color label table to disk.
   */
  void WriteASCIIColorLabels(const char* filename);

  /**
   * Cut the segmentation using a plane and relabed the segmentation
   * on the side of that plane
   */
  void RelabelSegmentationWithCutPlane(const Vector3d &plane, bool dZero, 
                       LabelType newlabel);

  /**
   * Compute the intersection of the segmentation with a ray
   */
  int GetRayIntersectionWithSegmentation(const Vector3d &point, 
                     const Vector3d &ray, 
                     Vector3i &hit) const;

  /**
   * Deep copy a region of interest from this class 
   * to another instance of this class (copies labels 
   * and images)
   */
  void DeepCopyROI(Vector3i ul, Vector3i lr, IRISImageData &target);


  /**
   * Access the greyscale image (read only access is allowed)
   */
  GreyWrapperType* GetGrey() const {
    assert(m_GreyWrapper);
    return m_GreyWrapper;
  }

  /**
   * Access the segmentation image (read only access allowed 
   * to preserve state)
   */
  LabelWrapperType* GetSegmentation() const {
    assert(m_GreyWrapper && m_LabelWrapper);
    return m_LabelWrapper;
  }

  /** 
   * Get the extents of the image volume
   */
  Vector3i GetVolumeExtents() const {
    assert(m_GreyWrapper != NULL);
    assert(m_GreyWrapper->GetSize() == m_Size);
    return m_Size;
  }

  /**
   * Get the scaling of the voxels
   */
  Vector3f GetVoxelScaleFactor();

  /**
   * Set the grey image (read important note).
   * 
   * Note: this method replaces the internal pointer to the grey image
   * by the pointer that is passed in.  That means that the caller should relinquish
   * control of this pointer and that the IRISImageData class will dispose of the
   * pointer properly. 
   */
  void SetGrey(GreyWrapperType *inWrapper);

  /**
   * This method sets the segmentation image (see note for SetGrey).
   */
  void SetSegmentation(LabelWrapperType *inWrapper);

  /**
   * Set voxel in segmentation image
   */
  void SetSegmentationVoxel(const Vector3i &index, LabelType value);

  /**
   * Check validity of greyscale image
   */
  bool IsGreyLoaded();

  /**
   * Check validity of segmentation image
   */
  bool IsSegmentationLoaded();

  /**
   * Compute histogram of the segmentation image and save it
   * to some sort of a file.  
   * TODO: Implement a GUI for this, with a save button
   */
  int CountVoxels(const char *filename);

  /**
   * Set the cursor (crosshairs) position, in pixel coordinates
   */
  virtual void SetCrosshairs(const Vector3i &crosshairs);


protected:
  // Wrapper around the grey-scale image
  GreyWrapperType *m_GreyWrapper;

  // Wrapper around the segmentatoin image
  LabelWrapperType *m_LabelWrapper;

  // A table of color labels
  ColorLabel m_ColorLabels[MAX_COLOR_LABELS];

  // Number of active color labels
  unsigned int m_ColorLabelCount;

  // Dimensions of the images (must match) 
  Vector3i m_Size;

  // Color label initialization methods
  void InitializeColorLabels();
};

#endif