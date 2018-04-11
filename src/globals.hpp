#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <QString>
#include <QStringList>

///////////////////////////
// Global values in rrwc //
///////////////////////////

// valid image types
static const QStringList VALID_IMAGE_TYPES = {"*.jpg", "*.JPG"};

// status bar
static const QString STATUS_TOTAL = "Total: %p%";
static const QString STATUS_SORTING = "Sorting with Exif information: %p%";
static const QString STATUS_ENTRY_LIST = "Generating image lists: %p%";

// sort modes
static const QString SORT_EXIF = "Exif date";
static const QString SORT_FILENAME = "Filename";

// output values
static const QString OTP_FOLDER = "outputFolder";
static const QString OTP_RESIZE = "resize";
static const QString OTP_LENGTH = "length";
static const QString OTP_HEIGHT = "height";
static const QString OTP_RENAME = "rename";
static const QString OTP_RENAME_TEXT = "renameText";
static const QString OTP_COUNTER_START = "counterStart";
static const QString OTP_COUNTER_STEP = "counterStep";
static const QString OTP_COUNTER_DIGITS = "counterDigits";
static const QString OTP_WATERMARK = "watermark";
static const QString OTP_WATERMARK_TEXT = "watermarkText";
static const QString OTP_OPACITY = "opacity";
static const QString OTP_STRIP_METADATA = "stripMetadata";
static const QString OTP_COMMENT = "comment";
static const QString OTP_COMMENT_TEXT = "commentText";

// errors
static const QString ERR_OTP = "Error in Output [%1]: ";
static const QString ERR_IMAGE = "In image \"%1\": ";

static const QString ERR_NO_INPUT_FILES = "Error: The input folder does not contain any JPEG files.";
static const QString ERR_WATERMARK_TEXT = "Watermark file \"%1\" does not exist.";
static const QString ERR_SMALL_WATERMARK = "Watermark is too small to fit this resize setting. \
 \n-- Watermark dimentions are: Rows: %1 Columns: %2";
static const QString ERR_WATERMARK_FIT = "Watermark is too small to fit this image.";
static const QString ERR_WATERMARK_NO_RESIZE = "Watermark option can't be used without resize turned on.";
static const QString ERR_NO_EXIF = "No exif information found.";
static const QString ERR_FAIL = "Output: [%1]\t File: \"%2\" failed.";
static const QString ERR_WRONG_OUTPUT = "Output folder is not a directory, or you don't have the right permissions.";
static const QString ERR_WRONG_INPUT = "Input folder is not a directory, or you don't have the right permissions.";
static const QString ERR_NO_OUTPUT = "There are not outputs.";

static const QString ERR_PROFILE = "Error in profile [%1]: ";
static const QString ERR_PROFILE_MISSING_BRKT = "Outputs must be enclosed in \"{}\"";
static const QString ERR_PROFILE_SAVE = "Could not save profile under \"%1\". Check the permissions.";
static const QString ERR_WRONG_PROFILE = "Error: There is no such profile.";

// warnings
static const QString WAR_IMAGE = "Warning in image \"%1\": ";
static const QString WAR_EXIF_NOT_FOUD = "exif information not found.";

// progress
static const QString FILE_PROGRESS = "T:[%2] F:\"%3\"";
static const QString TIME_ENTRYLIST = "Filling the entry list took %1 ms";
static const QString TIME_TOTAL = "Total time elapsed: %1 ms";

// buttons
static const QString BUT_LOG = "Logs";
static const QString BUT_OUTPUTS = "Outputs";

#endif // GLOBALS_HPP
