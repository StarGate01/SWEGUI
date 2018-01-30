/**
 * @brief Errors that can occur in the renderer
 * @file Errors.hpp
*/

#ifndef RENDER_ERRORS_H
#define RENDER_ERRORS_H

#define ERROR_FILE      1   ///<Error while opening the file (IO)
#define ERROR_SELECT    2   ///<Error while selecting a layer
#define ERROR_STREAM    3   ///<Error while generating the stream
#define ERROR_IMAGE     4   ///<Error while converting the stream to an image
#define ERROR_SUCCESS   0   ///<No error

#endif