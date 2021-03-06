/*
   BAREOS® - Backup Archiving REcovery Open Sourced

   Copyright (C) 2016-2016 Bareos GmbH & Co. KG

   This program is Free Software; you can redistribute it and/or
   modify it under the terms of version three of the GNU Affero General Public
   License as published by the Free Software Foundation and included
   in the file LICENSE.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.
*/
/*
 * BAREOS Director -- archive.c -- responsible for doing archive jobs
 *
 * based on admin.c
 * Marco van Wieringen, June 2016
 */

#include "bareos.h"
#include "dird.h"

static const int dbglvl = 100;

bool do_archive_init(JCR *jcr)
{
   free_rstorage(jcr);
   if (!allow_duplicate_job(jcr)) {
      return false;
   }

   return true;
}

/*
 * Returns: false on failure
 *          true  on success
 */
bool do_archive(JCR *jcr)
{
   jcr->jr.JobId = jcr->JobId;

   jcr->fname = (char *)get_pool_memory(PM_FNAME);

   /*
    * Print Job Start message
    */
   Jmsg(jcr, M_INFO, 0, _("Start Archive JobId %d, Job=%s\n"), jcr->JobId, jcr->Job);

   jcr->setJobStatus(JS_Running);
   archive_cleanup(jcr, JS_Terminated);

   return true;
}

/*
 * Release resources allocated during archive.
 */
void archive_cleanup(JCR *jcr, int TermCode)
{
   char sdt[50], edt[50], schedt[50];
   char term_code[100];
   const char *term_msg;
   int msg_type;

   Dmsg0(dbglvl, "Enter archive_cleanup()\n");

   update_job_end(jcr, TermCode);

   if (!db_get_job_record(jcr, jcr->db, &jcr->jr)) {
      Jmsg(jcr, M_WARNING, 0, _("Error getting Job record for Job report: ERR=%s"), db_strerror(jcr->db));
      jcr->setJobStatus(JS_ErrorTerminated);
   }

   msg_type = M_INFO;                 /* by default INFO message */
   switch (jcr->JobStatus) {
   case JS_Terminated:
      term_msg = _("Archive OK");
      break;
   case JS_FatalError:
   case JS_ErrorTerminated:
      term_msg = _("*** Archive Error ***");
      msg_type = M_ERROR;          /* Generate error message */
      break;
   case JS_Canceled:
      term_msg = _("Archive Canceled");
      break;
   default:
      term_msg = term_code;
      sprintf(term_code, _("Inappropriate term code: %c\n"), jcr->JobStatus);
      break;
   }

   bstrftimes(schedt, sizeof(schedt), jcr->jr.SchedTime);
   bstrftimes(sdt, sizeof(sdt), jcr->jr.StartTime);
   bstrftimes(edt, sizeof(edt), jcr->jr.EndTime);

   Jmsg(jcr, msg_type, 0, _("BAREOS " VERSION " (" LSMDATE "): %s\n"
        "  JobId:                  %d\n"
        "  Job:                    %s\n"
        "  Scheduled time:         %s\n"
        "  Start time:             %s\n"
        "  End time:               %s\n"
        "  Termination:            %s\n\n"),
        edt,
        jcr->jr.JobId,
        jcr->jr.Job,
        schedt,
        sdt,
        edt,
        term_msg);

   Dmsg0(dbglvl, "Leave archive_cleanup()\n");
}
