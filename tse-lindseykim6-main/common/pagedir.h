/**************** pagedir.h ****************/
/* Lindsey Kim
*
* a library for pagedirectory functions
*/

#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"

/**************** pagedir_init ****************/
/* Initializes a directory with a .crawler file.
 *
 * Caller provides:
 *    the (existing) directory in which to write downloaded webpages.
 * We return:
 *   true if file properly closes, false if error
 *
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* saves a webpage into a page directory
 *
 * Caller provides:
 *    the webpage, the (existing) directory in which to write downloaded webpages, the docID
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_load ****************/
/* loads the contents of the page directory into an index
 *
 * Caller provides:
 *   the (existing) directory of files, the docID
 *
 */
webpage_t* pagedir_load(char* directory, const int docID);

/**************** pagedir_validate ****************/
/* validate if page directory is a valid crawler directory with .crawler file starting at file 1
 *
 * Caller provides:
 *    the (existing) directory from which to create the index
 *
 */
bool pagedir_validate(char* pageDirectory);
