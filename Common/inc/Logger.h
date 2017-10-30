/* ====================================================================
 * File: Logger.h
 * Created: 04/13/09
 * Modified: 05/08/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef LOGGER_H
#define LOGGER_H

#define _DEBUG

#ifdef _DEBUG

// INCLUDE FILES
#include <flogger.h>
#include <eikenv.h>

// log files are stored to KDebugLogDirFull folder
_LIT(KDebugLogDependency,       "C:\\Logs\\");
_LIT(KDebugLogDoubleBackSlash,  "\\");

_LIT( KDebugLogDir,               "Tetris2");
_LIT( KDebugLogFile,              "Tetris2.log");
_LIT( KDebugLogEnterFn,           "Tetris2: -> %S" );
_LIT( KDebugLogLeaveFn,           "Tetris2: <- %S" );
_LIT( KDebugLogTimeFormatString,  " [%H:%T:%S:%*C3]" );

// MACROS

/**
* Use this macro in order to initialize logger :
* - create log directory,
* - write version information into the log file
*/
#define _LOG_CREATE { TFileName path( KDebugLogDependency ); path.Append( KDebugLogDir ); path.Append( KDebugLogDoubleBackSlash ); RFs fs; CleanupClosePushL(fs); User::LeaveIfError( fs.Connect() ); fs.MkDirAll( path ); CleanupStack::PopAndDestroy( &fs ); }

/**
* Use this function at any points of a function for logging the current state.
* @param a String to be written into logfile about the current state
*/
#define _LOG_WRITE( a ) { _LIT( temp, a ); RFileLogger::Write( KDebugLogDir, KDebugLogFile, EFileLoggingModeAppend, temp ); RDebug::Print(temp);}

/**
* Use this function at any points of a function for logging the current state.
* You can use printf-like formats, but with only one parameter to be substituted..
* @param a Format string,
* @param b Parameter to be substituted.
*/
#define _LOG_WRITE_FORMAT( a, b ) { _LIT( temp, a ); RFileLogger::WriteFormat( KDebugLogDir, KDebugLogFile, EFileLoggingModeAppend, temp, b ); RDebug::Print( temp, b);}

/**
* Use this function at any points of a function for logging the current state.
* @param a String to be written into logfile about the current state
*/
#define _LOG_WRITE_TDES( a ) { RFileLogger::Write( KDebugLogDir, KDebugLogFile, EFileLoggingModeAppend, a ); RDebug::Print( a );}

/**
* Use this function at any points of a function for logging the current state.
* The current date/time stamp is written besides the string you give.
* @param a String to be written into logfile about the current state
*/
#define _LOG_WRITE_TIMESTAMP( a ) { _LIT( temp, a ); TTime time; time.HomeTime(); TBuf<256> buffer; time.FormatL( buffer, KDebugLogTimeFormatString ); buffer.Insert( 0, temp ); RFileLogger::Write( KDebugLogDir, KDebugLogFile, EFileLoggingModeAppend, buffer); RDebug::Print( buffer );}

#else // _DEBUG

// Empty macros
#define _LOG_CREATE
#define _LOG_ENTERFN( a )
#define _LOG_LEAVEFN( a )
#define _LOG_WRITE( a )
#define _LOG_WRITE_FORMAT( a, b )
#define _LOG_WRITE_TIMESTAMP( a )
#define _LOG_WRITE_TDES( a )
#endif // _DEBUG

#endif // LOGGER_H

// End of file
