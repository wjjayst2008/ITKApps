/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    UserInterfaceLogic.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __UserInterfaceLogic_h_
#define __UserInterfaceLogic_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "SNAPCommonUI.h"
#include "UserInterface.h"

// Necessary forward references
class IRISApplication;
class GlobalState;
class HelpViewerLogic;
class IntensityCurveUILogic;
class PreprocessingUILogic;
class RestoreSettingsDialogLogic;
class SnakeParametersUILogic;
class SystemInterface;

class GreyImageIOWizardLogic;
class SegmentationImageIOWizardLogic;
class PreprocessingImageIOWizardLogic;
class SliceWindowCoordinator;
class SimpleFileDialogLogic;
class ResizeRegionDialogLogic;
class SNAPAppearanceSettings;
class AppearanceDialogUILogic;

template <class TFlag> class FLTKWidgetActivationManager;
//template<class TPixel> class ImageIOWizardLogic;
//class SegmentationImageIOWizardLogic;

#include <time.h>

// ITK forward references
namespace itk {
  template <class TObject> class SimpleMemberCommand;
  template <class TObject> class MemberCommand;
};


/**
 * \class UserInterfaceLogic
 * \brief Logic for the main user interface.
 *
 * This class implements virtual function from
 * the GUI class that is generated by fluid.
 * The idea is to have virtual functions for
 * all the GUI callbacks, and then override
 * them in this class, allowing us to avoid
 * coding inside fluid, since it's annoying.
 *
 * Note that the GUI class contains all the
 * windows (the IRIS window, the 3D Snake window,
 * and all the dialogs).
 */
class UserInterfaceLogic : public UserInterface 
{
public:

  /** Constructor, calls GUI constructor, calls init */
  UserInterfaceLogic(IRISApplication *iris,SystemInterface *system);

  /** Destructor */
  virtual ~UserInterfaceLogic();

  /** Hides the snake window and shows the iris window */
  void ShowIRIS();

  /** Resets the region of interest to contain the entire volume */
  void OnResetROIAction();

  /**
   *
   * DESCRIPTION:
   * callback for the snake button
   * this function takes care of initializing
   * the global variable roi_data, sets vox_data
   * to point to roi_data, sets up the snake window
   * and calls ShowSNAP()
   *
   * PRECONDITIONS:
   * - global_state must have a valid ROI
   *
   * POSTCONDITIONS:
   * - snake window is displayed, IRIS is hidden
   */
  void OnSnakeStartAction();

  /**
   *
   * DESCRIPTION:
   * Callback for showing the dialog box for preprocessing the grey data.
   * Depending on the type of snake chosen either the dialog for thresholding
   * or edge detection is displayed.
   *
   * PRECONDITIONS:
   * - main snake window is initialized and active
   *
   * POSTCONDITIONS:
   * - preprocessing dialog appears on screen
   */
  void OnPreprocessAction();
  
  /** Callback for when the preprocessing completes */
  void OnPreprocessClose();

  /**
   *
   * DESCRIPTION:
   * callback to set bubbles for snake initialization
   * - sets a bubble at the current crosshairs position with the radius
   * equal to the current value on the  radius slider
   * - adds the bubble to the Active Bubbles browser
   *
   * PRECONDITIONS:
   * - Snake GUI is active
   * - Crosshairs position is within allowed boundaries
   *
   * POSTCONDITIONS:
   * - all previously set bubbles as well as the new bubble are displayed
   * in 2d windows
   * - Active Bubbles browser contains all previously set bubbles as
   * well as the new bubble and their parameters are displayed in the browser
   */
  void OnAddBubbleAction();

  /**
   *
   * DESCRIPTION:
   * callback to remove a bubble
   * - removes a highlighted bubble from Active Bubbles browser
   *
   * PRECONDITIONS:
   * - Snake GUI is active
   * - Active Bubbles browser contains at least one bubble
   *
   * POSTCONDITIONS:
   * - highlighted bubble has been removed from Active Bubbles browser
   * and is no longer displayed in 2d windows
   * - Active Bubbles browser contains all remaining bubbles and their
   * parameters are displayed in the browser
   * - all remaining bubbles are displayed in 2d windows
   *
   */
  void OnRemoveBubbleAction();

  /**
   *
   * DESCRIPTION:
   * callback to browse through the bubbles in the Active Bubbles browser
   * - highlights a line clicked in the browser window and sets the radius
   * slider to the value of the highlighted bubble's radius
   *
   * PRECONDITIONS:
   * - Snake GUI is active
   * - Active Bubbles browser contains at least one bubble
   *
   * POSTCONDITIONS:
   * - the line clicked in the browser window is highlighted
   * - value of m_HighlightedBubble integer variable is set to the
   * number of the line highlighted in the browser window
   * - radius slider value is reset to the value of the highlighted
   * bubble's radius
   */
  void OnActiveBubblesChange();

  /**
   *
   * DESCRIPTION:
   * callback to set the radius of a bubble highlighted in the
   * Active Bubbles browser
   * - sets the radius of the highlighted bubble to the current value of
   * the radius slider
   * - only displays the current value of the radius slider if no bubble is
   * highlighted or if the Active Bubbles browser is empty
   *
   * PRECONDITIONS:
   * - Snake GUI is active
   *
   * POSTCONDITIONS:
   * - current value of the radius slider is displayed
   * - radius of the highlighted bubble is set to the current value of
   * the radius slider
   * - visualisation of the highlighted bubble in 2d windows changes
   * its radius correspondingly
   * - all remaining bubbles are left unchanged
   */
  void OnBubbleRadiusChange();

  /** 
   * Opens the snake params dialog, allowing the user to modify snake params
   */
  void OnSnakeParametersAction();

  /**
   *
   * DESCRIPTION:
   * callback for accepting a snake segmentation
   *
   * PRECONDITIONS:
   * - roi_data is valid
   * - roi_data.snakeImageData is initialized
   *
   * POSTCONDITIONS:
   * - roi_data.snakeImageData has been merged with full_data.segImageData
   * - roi_data is deleted
   * - IRIS window is shown
   *
   */
  void OnAcceptSegmentationAction();

  /**
   *
   * DESCRIPTION:
   * callback for canceling a snake segmentation
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * - snake window is hidden, IRIS is shown
   * - roi_data is deleted
   *
   */
  void OnCancelSegmentationAction();


  /**
   *
   * DESCRIPTION:
   * disables or enables the UpdateMesh button in snake win depending
   * on the state of the checkbox
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   *
   */
  void OnContinuousViewUpdateChange();

  /**
   *
   * DESCRIPTION:
   * Switches to InOut snake mode
   *
   * PRECONDITIONS:
   * - Snake UI is active
   *
   * POSTCONDITIONS:
   * - all current preprocessed data is invalidated
   */
  void OnInOutSnakeSelect();

  /**
   *
   * DESCRIPTION:
   * Switches to Edge snake mode
   *
   * PRECONDITIONS:
   * - Snake UI is active
   *
   * POSTCONDITIONS:
   * - all current preprocessed data is invalidated
   */
  void OnEdgeSnakeSelect();

  /**
   *
   * DESCRIPTION:
   * callback for accepting the snake initialization.  The active
   * bubbles are voxelized.  roi_data.segImageData and snakeInitImageData
   * are initialized and synchronized.  The bubble voxels are put into
   * snakeInitImageData. If the grey data has been preprocessed, and
   * some type of initialization is present in snakeInitImageData (either
   * a bubble or pre-existing segmentation data from segImageData), the
   * snake can now be run.
   *
   * PRECONDITIONS:
   * - snake window is up and running
   *
   * POSTCONDITIONS:
   * - global_state.GetSnakeActive is true
   *
   */
  void OnAcceptInitializationAction();

  /**
   *
   * DESCRIPTION:
   * callback for restarting the snake initialization.  Any current snake
   * state is forgotten, the snake controls are disabled, the segmentation
   * shown in the 2D windows reverts back to the segImageData, and the bubbles
   * are shown again.  The initialization widgets (bubbles, preproc) are enabled
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * - global_state.GetSnakeActive is false
   *
   */
  void OnRestartInitializationAction();

  /**
   * User wants to go back to the preprocessing page from the segmentation 
   * initialization page.  This callback will simply flip the page and clear
   * the bubbles
   */
  void OnRestartPreprocessingAction();

  /** The user has finished preprocessing the image and flips to the next 
   * page in the wizard.  The speed image must be valid for this button to be
   * active */
  void OnAcceptPreprocessingAction();

  /**
   *
   * DESCRIPTION:
   * callback for the snake "vcr" rewind button.  Restarts the snake
   * at the same state as when AcceptInitialization button was pressed
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   *
   */
  void OnSnakeRewindAction();

  /**
   *
   * DESCRIPTION:
   * simply sets a flag so that the play button knows to stop
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   *
   */
  void OnSnakeStopAction();

  /**
   *
   * DESCRIPTION:
   * continuously steps the snake until m_SnakeIsRunning flag is unset
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   *
   */
  void OnSnakePlayAction();

  /**
   *
   * DESCRIPTION:
   * runs the snake for one step.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * - 2D displays are updated according to the current snake state
   * - if updatecontinuously checkbox is checked, 3D display is updated
   *
   */
  void OnSnakeStepAction();

  /**
   *
   * DESCRIPTION:
   * callback for choice box of how many snake iterations to run per "step"
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * m_SnakeStepSize is set
   *
   */
  void OnSnakeStepSizeChange();

  /**
   *
   * DESCRIPTION:
   * returns a pointer to an array of Bubble objects contained in
   * the Active Bubbles browser
   *
   * PRECONDITIONS:
   * - Snake GUI is active
   *
   * POSTCONDITIONS:
   * - a pointer to an array of Bubble objects contained in
   * the Active Bubbles browser is returned
   * - returns NULL if Active Bubbles browser is empty
   */
  Bubble* GetBubbles();

  /**
   *
   * DESCRIPTION:
   * returns the number of bubbles contained in the Active Bubbles
   * browser
   *
   * PRECONDITIONS:
   * - Snake GUI is active
   *
   * POSTCONDITIONS:
   * - the number of bubbles in the Active Bubbles browser is returned
   * - returns zero if the browser is empty
   */
  int GetNumberOfBubbles();

  /**
   * Pops up a dialog to choose a preprocessed data file
   * pre: LoadPreproc_win is active
   * post: header information is filled in in the dialog
   * void LoadPreprocSelectCallback();

   * Fills the header information in the Load Preprocessed data dialog
   * pre: the filename field contains a valid filename
   * post: header information is filled in
   * void SelectPreprocFileCallback();

   * Implements action performed when user presses OK
   * in the Load Preprocessed data dialog
   * PRE: nothing
   * POST: the preprocessed in roi_data is valid
   * AUTHORS: Talbert and Scheuermann
   * void LoadPreprocessedDataCallback();

   * Implements callback for Save->Preprocessed
   * menu item
   * PRE: nothing
   * POST: save dialog appears
   * void SavePreprocessedData_Callback();

   * Implements callback for Okay button in the
   * Save Preprocessed dialog box
   * PRE: nothing
   * POST: If a valid filename was supplied then
   * the preprocessed data set is saved to the
   * specified file
   * void OkaySavePreproc_button_Callback();

   * callback of the radio button for selecting the grey data
   * for display
   */
  void OnSNAPViewOriginalSelect();

  /**
   * callback of the radio button for selecting the preprocessed data
   * for display
   */
  void OnViewPreprocessedSelect();

  /**
   *
   * DESCRIPTION:
   * close both windows so FL::run returns, and program exits
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * - program will exit
   *
   */
  void OnMenuQuit();

  // Color label callbacks
  void InitColorMap(bool resetCurrentAndDrawOverLabels = true);
  void UpdateColorChips();
  void OnDrawingLabelUpdate();
  void OnDrawOverLabelUpdate();
  
  void RedrawWindows();
  void ResetScrollbars();
  void UpdateImageProbe();
  void UpdateMainLabel();
  // void Activate3DAccept(bool on);
  void UpdateEditLabelWindow();
  void UpdatePositionDisplay(int id);

  /** Set the current interaction mode */
  void SetToolbarMode(ToolbarModeType mode);
  
  /** Set the current 3D interaction mode */
  void SetToolbarMode3D(ToolbarMode3DType mode);

  /** Get the pointer to the driving application */
  irisGetMacro(Driver,IRISApplication *);

  /** Get the pointer to the system interface */
  irisGetMacro(SystemInterface,SystemInterface *);

  /** Get the reference to the appearance settings */
  irisGetMacro(AppearanceSettings, SNAPAppearanceSettings *);

  /** Update the user interface after loading a new grey image  */
  void OnGreyImageUpdate();

  /** Called before unloading a grey image, saves settings associated with it */
  void OnGreyImageUnload();

  /** Update the user interface after loading a new segmentation image  */
  void OnSegmentationImageUpdate();

  /** Update the user interface after loading a new labels file  */
  void OnSegmentationLabelsUpdate(bool resetCurrentAndDrawOverLabels);

  /** Update the user interface after loading a new preprocessing image  */
  void OnSpeedImageUpdate();

  // Splash screen functions
  void ShowSplashScreen();
  void HideSplashScreen();
  void UpdateSplashScreen(const char *message);
  
  /** A utility to center one window inside another */
  static void CenterChildWindowInParentWindow(Fl_Window *childWindow,
                                              Fl_Window *parentWindow);
  void CenterChildWindowInMainWindow(Fl_Window *childWindow);

  // Zoom/pan management callbacks
  void OnResetView2DAction(unsigned int window);
  void OnResetAllViews2DAction();
  void OnLinkedZoomChange();
  void OnZoomPercentageChange();

  // Internal callback used to update the zoom percentage displayed
  void OnZoomUpdate();

  // Internal callback for when the crosshairs position changes
  void OnCrosshairPositionUpdate();

  // Internal method called when slices need to be re-connected to the image,
  // i.e., when a new image is loaded or the image-display geometry changes
  void OnImageGeometryUpdate();

  /** Get the object used to coordinate zoom in slice windows */
  irisGetMacro(SliceCoordinator,SliceWindowCoordinator *);

  // Polygon button callbacks
  void OnAcceptPolygonAction(unsigned int window);
  void OnInsertIntoPolygonSelectedAction(unsigned int window);
  void OnDeletePolygonSelectedAction(unsigned int window);
  void OnPastePolygonAction(unsigned int window);
  void OnPolygonStateUpdate(unsigned int id);

  // IRIS: 3D Window callbacks
  void OnIRISMeshUpdateAction();
  void OnIRISMeshAcceptAction();
  void OnIRISMeshResetViewAction();
  void OnIRISMeshEditingAction();
  void OnIRISMeshDisplaySettingsUpdate();
   
  // SNAP: 3D Window callbacks
  void OnSNAPMeshUpdateAction();
  void OnSNAPMeshResetViewAction();
  void OnSNAPMeshContinuousUpdateAction();

  // Method called when user tries to close the window
  void OnMainWindowCloseAction();
  
  // Show the help system
  void ShowHTMLPage(const char *link);


protected:

  /**
   *
   * DESCRIPTION:
   * Initializes everything so the IRIS window can be shown.
   * This method is essentially the same as IRIS2000, but
   * we added a couple initializations of snake window stuff.
   *
   * PRECONDITIONS:
   * - basically assumes it's going to be called in the constructor.
   *
   * POSTCONDITIONS:
   * - the IRIS window can now be shown
   */
  void InitializeUI();

  /**
   * UserInterfaceLogic::ShowSNAP
   *
   * DESCRIPTION:
   *   hides the iris window and shows the snake window
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * - the IRIS window is hidden and the snake window is shown
   *
   */
  void ShowSNAP();

  /**
   * DESCRIPTION
   * Runs the snake, and updates the GUI
   *
   * PRE
   * snake must be ready to run
   *
   * POST
   *
   * RETURNS
   * 1 if running is successful, 0 if an error occurred
   */
  // int RunSnake();

  // Callbacks to the simple file dialog
  void OnLoadLabelsAction();
  void OnSaveLabelsAction();
  void OnWriteVoxelCountsAction();

  void ChangeLabelsCallback();
  void OnSliceSliderChange(int id);

  // Menu item callbacks
  void OnMenuLoadGrey();
  void OnMenuLoadSegmentation();
  void OnMenuLoadLabels();
  void OnMenuSaveGreyROI();
  void OnMenuSaveSegmentation();
  void OnMenuSaveLabels();
  void OnMenuWriteVoxelCounts();
  void OnMenuIntensityCurve();
  void OnMenuLoadPreprocessed();  
  void OnMenuSavePreprocessed();  
  void OnLoadPreprocessedImageAction();
  void OnMenuLoadAdvection();
  void OnMenuImageInfo();

  // Save a slice
  void OnMenuExportSlice(unsigned int iSlice);

  // Display options callbacks
  void OnMenuShowDisplayOptions();
  
  // Opacity slider callbacks
  void OnSNAPLabelOpacityChange();
  void OnIRISLabelOpacityChange();

  // Help system callbacks
  void OnLaunchTutorialAction();

  // Set the active page in the segmentation pipeline
  void SetActiveSegmentationPipelinePage(unsigned int page);

  // Restoring settings
  void OnRestoreSettingsAction();
  void OnDoNotRestoreSettingsAction();

  // Methods to tweak window positions
  void OnIRISWindowFocus(unsigned int i);

  // Image Info Window Callbacks
  void OnCloseImageInfoAction();
  
  char *m_ChosedFile;

private:

  // Typedef for event callback commands
  typedef itk::SimpleMemberCommand<UserInterfaceLogic> SimpleCommandType;
  typedef itk::MemberCommand<UserInterfaceLogic> ProgressCommandType;

  // Pointer to the driving IRIS application object
  IRISApplication *m_Driver;

  // Pointer to the system interface object
  SystemInterface *m_SystemInterface;

  // Settings related to the cosmetic appearance of the application
  SNAPAppearanceSettings *m_AppearanceSettings;

  // Set of logical states that the UI may encounter
  enum UIStateFlags {
    UIF_NULL,
    UIF_GRAY_LOADED,
    UIF_IRIS_ACTIVE,
    UIF_IRIS_WITH_GRAY_LOADED,
    UIF_IRIS_MESH_DIRTY,
    UIF_IRIS_MESH_ACTION_PENDING, 
    UIF_IRIS_ROI_VALID,
    UIF_LINKED_ZOOM,

    UIF_SNAP_ACTIVE,
    UIF_SNAP_PAGE_PREPROCESSING,
    UIF_SNAP_PAGE_BUBBLES,
    UIF_SNAP_PAGE_SEGMENTATION,
    UIF_SNAP_PREPROCESSING_ACTIVE,
    UIF_SNAP_PREPROCESSING_DONE,
    UIF_SNAP_DIALOG_PARAMETERS,
    UIF_SNAP_DIALOG_PREPROCESSING,
    UIF_SNAP_SNAKE_RUNNING,
    UIF_SNAP_SNAKE_INITIALIZED,
    UIF_SNAP_SNAKE_EDITABLE,
    UIF_SNAP_SPEED_AVAILABLE,
    UIF_SNAP_MESH_DIRTY,
    UIF_SNAP_MESH_CONTINUOUS_UPDATE
  };

  // Widget activation manager - simplifies the headache of keeping widgets
  // activated and deactivated
  FLTKWidgetActivationManager<UIStateFlags> *m_Activation; 

  // Bubble highlighted in the browser
  int m_HighlightedBubble;

  // how many snake iterations per step
  int m_SnakeStepSize;

  // The callback command used in the (complicated) snake VCR pipeline
  itk::SmartPointer<SimpleCommandType> m_PostSnakeCommand;

  // The main window label
  std::string m_MainWindowLabel;

  /** Wizard used to load grey image files */
  GreyImageIOWizardLogic *m_WizGreyIO;

  /** Wizard used to load and save segmentation image files */
  SegmentationImageIOWizardLogic *m_WizSegmentationIO;

  /** Wizard used to load and save preprocessing image files */
  PreprocessingImageIOWizardLogic *m_WizPreprocessingIO;

  /** UI object used for handling Curve editing */
  IntensityCurveUILogic *m_IntensityCurveUI;

  /** Preprocessing UI object */
  PreprocessingUILogic *m_PreprocessingUI;

  /** Parameter setting UI object */
  SnakeParametersUILogic *m_SnakeParametersUI;

  /** A restore settings dialog */
  RestoreSettingsDialogLogic *m_DlgRestoreSettings;

  /** A dialog for resampling the image */
  ResizeRegionDialogLogic *m_DlgResampleRegion;

  /** A dialog for showing display options */
  AppearanceDialogUILogic *m_DlgAppearance;

  /** Help window */
  HelpViewerLogic *m_HelpUI;

  /** A coordinator for the slice windows */
  SliceWindowCoordinator *m_SliceCoordinator;

  /** Splash screen timer */
  clock_t m_SplashScreenStartTime;

  /** Save/load labels dialog */
  SimpleFileDialogLogic *m_DlgLabelsIO;

  /** Write voxels dialog */
  SimpleFileDialogLogic *m_DlgVoxelCountsIO;

  // Intensity curve update callback (uses ITK event system)
  void OnIntensityCurveUpdate();

  /** Initialization subroutine that sets up the activation manager */
  void InitializeActivationFlags();

  /**
   * Some GUI state is shared between the windows, like opacity settings, 
   * toolbar mode. This function syncs the two.
   */
  void SyncIRISToSnake();
  
  /**
   * Some GUI state is shared between the windows, like opacity settings, 
   * toolbar mode. This function syncs the two.
   */
  void SyncSnakeToIRIS();

  /** Common code for cancelling and accepting the segmentation */
  void CloseSegmentationCommon();

  /** This method is used to figure out which image axis corresponds to a
   * given display window */
  unsigned int GetImageAxisForDisplayWindow(unsigned int window);

  /** Progress callback for ITK methods. Will show the progress bar and fill
   * the bar as necessary */
  void OnITKProgressEvent(itk::Object *source, const itk::EventObject &event);

  // This method is called when the snake image has changed.
  void OnSnakeUpdate();

  /* Command used for progress tracking */
  itk::SmartPointer<ProgressCommandType> m_ProgressCommand;

  // A function used to run the snake in the background
  friend void fnSnakeIdleFunction(void *userData);
};

#endif

/*
 *Log: UserInterfaceLogic.h
 *Revision 1.23  2005/03/08 03:12:51  pauly
 *BUG: Minor bugfixes in SNAP, mostly to the user interface
 *
 *Revision 1.22  2004/09/14 14:11:11  pauly
 *ENH: Added an activation manager to main UI class, improved snake code, various UI fixes and additions
 *
 *Revision 1.21  2004/09/08 12:09:46  pauly
 *ENH: Adapting SNAP to work with stop-n-go function in finite diff. framewk
 *
 *Revision 1.20  2004/08/26 18:29:19  pauly
 *ENH: New user interface for configuring the UI options
 *
 *Revision 1.19  2004/08/03 23:26:32  ibanez
 *ENH: Modification for building in multple platforms. By Julien Jomier.
 *
 *Revision 1.18  2004/07/30 15:56:34  jjomier
 *FIX: Warning, disabling 4786 on MSVC
 *
 *Revision 1.17  2004/07/29 14:00:36  pauly
 *ENH: A new interface for changing the appearance of SNAP
 *
 *Revision 1.16  2004/07/24 19:00:06  pauly
 *ENH: Thumbnail UI for slice zooming
 *
 *Revision 1.15  2004/07/21 18:17:45  pauly
 *ENH: Enhancements to the way that the slices are displayed
 *
 *Revision 1.14  2004/03/19 00:54:48  pauly
 *ENH: Added the ability to externally load the advection image
 *
 *Revision 1.13  2004/01/19 22:17:52  pauly
 *ENH: Final touches before the 1.0 release
 *
 *Revision 1.12  2003/12/07 19:48:41  pauly
 *ENH: Resampling, multiresolution
 *
 *Revision 1.11  2003/11/29 17:06:48  pauly
 *ENH: Minor Help issues
 *
 *Revision 1.10  2003/11/10 00:27:26  pauly
 *FIX: Bug with linear interpolation in PDE solver
 *ENH: Help viewer and tutorial
 *
 *Revision 1.9  2003/10/09 22:45:14  pauly
 *EMH: Improvements in 3D functionality and snake parameter preview
 *
 *Revision 1.8  2003/10/07 00:37:27  jjomier
 *ENH: Added cygwin support
 *
 *Revision 1.7  2003/10/06 12:30:00  pauly
 *ENH: Added history lists, remembering of settings, new snake parameter preview
 *
 *Revision 1.6  2003/10/02 14:55:52  pauly
 *ENH: Development during the September code freeze
 *
 *Revision 1.2  2003/09/13 15:18:01  pauly
 *FIX: Got SNAP to work properly with different image orientations
 *
 *Revision 1.1  2003/09/11 13:51:01  pauly
 *FIX: Enabled loading of images with different orientations
 *ENH: Implemented image save and load operations
 *
 *Revision 1.5  2003/08/28 22:58:30  pauly
 *FIX: Erratic scrollbar behavior
 *
 *Revision 1.4  2003/08/28 14:37:09  pauly
 *FIX: Clean 'unused parameter' and 'static keyword' warnings in gcc.
 *FIX: Label editor repaired
 *
 *Revision 1.3  2003/08/27 14:03:23  pauly
 *FIX: Made sure that -Wall option in gcc generates 0 warnings.
 *FIX: Removed 'comment within comment' problem in the cvs log.
 *
 *Revision 1.2  2003/08/27 04:57:47  pauly
 *FIX: A large number of bugs has been fixed for 1.4 release
 *
 *Revision 1.1  2003/07/12 04:46:50  pauly
 *Initial checkin of the SNAP application into the InsightApplications tree.
 *
 *Revision 1.17  2003/07/12 01:34:18  pauly
 *More final changes before ITK checkin
 *
 *Revision 1.16  2003/07/11 23:28:10  pauly
 **** empty log message ***
 *
 *Revision 1.15  2003/07/10 14:30:26  pauly
 *Integrated ITK into SNAP level set segmentation
 *
 *Revision 1.14  2003/06/23 23:59:32  pauly
 *Command line argument parsing
 *
 *Revision 1.13  2003/06/14 22:42:06  pauly
 *Several changes.  Started working on implementing the level set function
 *in ITK.
 *
 *Revision 1.12  2003/06/08 23:27:56  pauly
 *Changed variable names using combination of ctags, egrep, and perl.
 *
 *Revision 1.11  2003/06/08 16:11:42  pauly
 *User interface changes
 *Automatic mesh updating in SNAP mode
 *
 *Revision 1.10  2003/05/22 17:36:19  pauly
 *Edge preprocessing settings
 *
 *Revision 1.9  2003/05/17 21:39:30  pauly
 *Auto-update for in/out preprocessing
 *
 *Revision 1.8  2003/05/14 18:33:58  pauly
 *SNAP Component is working. Double thresholds have been enabled.  Many other changes.
 *
 *Revision 1.7  2003/05/07 19:14:46  pauly
 *More progress on getting old segmentation working in the new SNAP.  Almost there, region of interest and bubbles are working.
 *
 *Revision 1.6  2003/05/05 12:30:18  pauly
 **** empty log message ***
 *
 *Revision 1.5  2003/04/25 02:58:29  pauly
 *New window2d model with InteractionModes
 *
 *Revision 1.4  2003/04/23 06:05:18  pauly
 **** empty log message ***
 *
 *Revision 1.3  2003/04/18 17:32:18  pauly
 **** empty log message ***
 *
 *Revision 1.2  2003/04/18 00:25:37  pauly
 **** empty log message ***
 *
 *Revision 1.1  2003/04/16 05:04:17  pauly
 *Incorporated intensity modification into the snap pipeline
 *New IRISApplication
 *Random goodies
 *
 *Revision 1.1  2003/03/07 19:29:47  pauly
 *Initial checkin
 *
 *Revision 1.1.1.1  2002/12/10 01:35:36  pauly
 *Started the project repository
 *
 *
 *Revision 1.33  2002/04/28 20:12:40  scheuerm
 *tiny documentation changes
 *
 *Revision 1.32  2002/04/28 17:29:43  scheuerm
 *Added some documentation
 *
 *Revision 1.31  2002/04/27 18:30:03  moon
 *Finished commenting
 *
 *Revision 1.30  2002/04/27 17:48:34  bobkov
 *Added comments
 *
 *Revision 1.29  2002/04/27 00:08:27  talbert
 *Final commenting run through . . . no functional changes.
 *
 *Revision 1.28  2002/04/26 17:37:13  moon
 *Fixed callback on save preproc dialog cancel button.
 *Fixed bubble browser output.  Position was zero-based, which didn't match the 2D
 *window slice numbers (1 based), so I changed the bubble positions to be cursor
 *position +1.
 *Disallowed starting snake window if current label in not visible.
 *Put in Apply+ button in threshold dialog, which changes seg overlay to be an
 *overlay of the positive voxels in the preproc data (a zero-level visualization).
 *Added more m_OutMessage and m_OutMessage messages.
 *
 *Revision 1.27  2002/04/24 19:50:23  moon
 *Pulled LoadGreyFileCallback out of GUI into UserInterfaceLogic, made modifications due
 *to change in ROI semantics.  Before, the ROI was from ul to lr-1, which is a bad
 *decision.  I changed everything to work with a ROI that is inclusive, meaning
 *that all voxels from ul through lr inclusive are part of the ROI. This involved
 *a lot of small changes to a lot of files.
 *
 *Revision 1.26  2002/04/23 19:28:20  bobkov
 *declared TweakROI method in UserInterfaceLogic.h to be public
 *left ColorLabel.h and ColorLabel.cpp unchanged
 *
 *Revision 1.25  2002/04/19 23:03:59  moon
 *Changed more stuff to get the snake params state synched with the global state.
 *Changed the range of ground in snake params dialog.
 *Removed the use_del_g stuff, since it's really not necessary, I found out.
 *
 *Revision 1.24  2002/04/19 20:34:58  moon
 *Made preproc dialogs check global state and only preproc if parameters have changed.
 *So no if you hit apply, then ok, it doesn't re process on the ok.
 *
 *Revision 1.23  2002/04/18 21:14:03  moon
 *I had changed the Cancel buttons to be Close on the Filter dialogs, and I changed
 *the names of the callbacks in GUI, but not in UserInterfaceLogic.  So I just hooked them
 *up so the dialogs get closed.
 *
 *Revision 1.22  2002/04/18 21:04:51  moon
 *Changed the IRIS window ROI stuff.  Now the ROI is always valid if an image is
 *loaded, but there is a toggle to show it or not.  This will work better with
 *Konstantin's addition of being able to drag the roi box.
 *
 *I also changed a bunch of areas where I was calling InitializeSlice for the 2D windows,
 *when this is not at all what I should have done.  Now those spots call
 *MakeSegTextureCurrent, or MakeGreyTextureCurrent.  This means that the view is not
 *reset every time the snake steps, the preproc/orig radio buttons are changed, etc.
 *
 *Revision 1.21  2002/04/16 18:54:33  moon
 *minor bug with not stopping snake when play is pushed, and then other
 *buttons are pushed.  Also added a function that can be called when the user
 *clicks the "X" on a window, but it's not what we want, I don't think.  The
 *problem is if the user clicks the "X" on the snake window when a "non modal"
 *dialog is up, all the windows close, but the program doesn't quit.  I think
 *it's a bug in FLTK, but I can't figure out how to solve it.
 *
 *Revision 1.20  2002/04/16 13:07:56  moon
 *Added tooltips to some widgets, made minor changes to enabling/disabling of
 *widgets, clearing 3D window when initialization is restarted in snake window,
 *changed kappa in edge preproc dialog to be [0..1] range instead of [0..3]
 *
 *Revision 1.19  2002/04/14 22:02:54  scheuerm
 *Changed loading dialog for preprocessed image data. Code restructuring
 *along the way: Most important is addition of
 *SnakeVoxDataClass::ReadRawPreprocData()
 *
 *Revision 1.18  2002/04/11 23:07:43  bobkov
 *Commented the Bubble class
 *Commented m_BtnAddBubble, m_BtnRemoveBubble,
 *m_BrsActiveBubbles and m_InBubbleRadius callbacks
 *Commented GetBubbles and GetNumberOfBubbles methodS
 *
 *Revision 1.17  2002/04/10 20:19:40  moon
 *got play and stop vcr buttons to work.
 *put in lots of comments.
 *
 *Revision 1.16  2002/04/09 18:59:33  moon
 *Put in dialog to change snake parameters.  Also implemented Rewind button, which
 *now restarts the snake.  It seems for now that changing snake parameters restarts
 *the snake.  I don't know if this is the way it has to be, or I just did something
 *wrong in snakewrapper.  I'll have to check with Sean.
 *
 *Revision 1.15  2002/04/08 13:32:35  talbert
 *Added a preprocessed save dialog box as well as a save preprocessed menu
 *option in the snake window.  Added the code necessary to implement the
 *GUI side of saving.
 *
 *Revision 1.14  2002/04/07 02:22:49  scheuerm
 *Improved handling of OK and Apply buttons in preprocessing dialogs.
 *
 *Revision 1.13  2002/04/04 15:30:09  moon
 *Put in code to get StepSize choice box filled with values and working.
 *AcceptSegment button callback puts snake seg data into full_data (IRIS)
 *Fixed a couple more UI cosmetic things.
 *
 *Revision 1.12  2002/04/03 22:12:07  moon
 *Added color chip, image probe, seg probe to snake window, although seg probe
 *maybe shouldn't be there.  added update continuously checkbox to 3Dwindow.
 *changes accept/restart to be on top of each other, and one is shown at a time,
 *which I think is more intuitive.
 *changed snake iteration field to be text output.  added callback for step size
 *choice.
 *
 *Revision 1.11  2002/03/27 17:59:40  moon
 *changed a couple things.  nothing big. a callback in .fl was bool return type
 *which didn't compile in windows. this is the version I think will work for a
 *demo for Kye
 *
 *Revision 1.10  2002/03/26 18:16:32  scheuerm
 *Added loading and display of preprocessed data:
 *- added vtkImageDeepCopy function
 *- added flags indicating which dataset to display in GlobalState
 *- added flag indicating whether to load gray or preprocessed data
 *  in the GUI class
 *
 *Revision 1.9  2002/03/25 02:15:57  scheuerm
 *Added loading of preprocessed data. It isn't being converted
 *to floats yet. It's not possible to actually display the data
 *right now.
 *
 *Revision 1.8  2002/03/24 19:27:46  talbert
 *Added callback the preprocess button to show dialog boxes for filtering.  Added callbacks for buttons in filtering dialog boxes.  Modified the AddBubbles callback so that the newest bubble is selected in the Bubble Browser.  m_OutAboutCompiled and ran to verify that new bubbles are selected and that the dialogs appear over the
 *3d window.  talbert s f
 *
 *Revision 1.7  2002/03/21 15:45:46  bobkov
 *implemented callbacks for buttons AddBubble and RemoveBubble, implemented callbacks for Radius slider and ActiveBubble browser, created methods getBubbles and getNumberOfBubbles   e
 *
 *Revision 1.6  2002/03/19 19:35:32  moon
 *added snakewrapper to makefile so it gets compiled. started putting in callback,
 *etc. for snake vcr buttons.  added snake object to IrisGlobals, instantiated in Main
 *
 *Revision 1.5  2002/03/19 17:47:10  moon
 *added some code to disable widgets, make the radio buttons work, etc. in the snake window.  fixed the quit callback from the snake window to work (crashed before)
 *changed the [accept/restart]bubble_button widgets to be acceptinitialization_button and added callbacks (empty).
 *
 *Revision 1.4  2002/03/08 14:23:48  moon
 *added comments
 *
 *Revision 1.3  2002/03/08 14:06:29  moon
 *Added Header and Log tags to all files
 **/
