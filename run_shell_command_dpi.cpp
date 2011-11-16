
/* Version 1.0, November 2011.

   See the README file for more information.

   Copyright (c) 2011 R. Diez

   This source file may be used and distributed without
   restriction provided that this copyright statement is not   
   removed from the file and that any derivative work contains 
   the original copyright notice and the associated disclaimer.
                                                             
   This source file is free software; you can redistribute it  
   and/or modify it under the terms of the GNU Lesser General  
   Public License version 3 as published by the Free Software Foundation.
                                                             
   This source is distributed in the hope that it will be      
   useful, but WITHOUT ANY WARRANTY; without even the implied  
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR     
   PURPOSE.  See the GNU Lesser General Public License for more
   details.                                                    
                                                             
   You should have received a copy of the GNU Lesser General   
   Public License along with this source; if not, download it  
   from http://www.gnu.org/licenses/
*/

// NOTE: The Verilator-generated forward declarations of the DPI methods like jtag_dpi_init()
//       must be visible to this file. If you are compiling this file as a standalone module,
//       you will have to include here the apropriate Verilator-generated header file.
//       Example:  #include "Vminsoc_bench.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // For system().

#include <stdexcept>


// We may have more error codes in the future, that's why the success value is zero.
// It would be best to return the error message as a string, but Verilog
// does not have good support for variable-length strings.
static const int RET_SUCCESS = 0;
static const int RET_FAILURE = 1;

static const char ERROR_MSG_PREFIX_INIT[] = "Error in run_shell_command_dpi(): ";


int run_shell_command_dpi ( int * const command_exit_code,
                            const char * const shell_cmd_to_run_formatted )
{
  try
  {
    if ( shell_cmd_to_run_formatted == NULL || strlen( shell_cmd_to_run_formatted ) <= 0 )
    {
      throw std::runtime_error( "Invalid shell_cmd_to_run_formatted parameter." );
    }

    // printf( "Shell command to run: %s\n", shell_cmd_to_run_formatted );
    
    const int child_status = system( shell_cmd_to_run_formatted );
      
    if ( -1 == child_status )
    {
      throw std::runtime_error( "Error running shell command." );
    }

    if ( ! WIFEXITED( child_status ) )
    {
      throw std::runtime_error( "The shell command terminated abnormally." );
    }

    *command_exit_code = WEXITSTATUS( child_status );
  }
  catch ( const std::exception & e )
  {
    // We should return this error string to the caller,
    // but Verilog does not have good support for variable-length strings.
    fprintf( stderr, "%s%s\n", ERROR_MSG_PREFIX_INIT, e.what() );
    fflush( stderr );

    return RET_FAILURE;
  }
  catch ( ... )
  {
    fprintf( stderr, "%sUnexpected C++ exception.\n", ERROR_MSG_PREFIX_INIT );
    fflush( stderr );

    return RET_FAILURE;
  }

  return RET_SUCCESS;
}
