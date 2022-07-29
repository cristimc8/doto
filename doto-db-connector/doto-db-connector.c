#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if(argc == 1) {
		printf("We need a task to store, along with an optional project, but the connector recieved 0 arguments.");
		return 1;
	}

	sqlite3 *dbHandle;
	sqlite3_stmt *res;
	char* errMsg = 0;
	int execStatus;

	if((execStatus = sqlite3_open("doto-db.sqlite", &dbHandle)) != SQLITE_OK) {
		fprintf(stderr, "An error occured: %s\n", sqlite3_errmsg(dbHandle));
		sqlite3_close(dbHandle);

		return 1;
	}

	int pid = -1;
	/* get the project id if project was supplied  */
	if(argc == 3) {
		const char* sql = "SELECT p.id FROM"
							"projects p WHERE p.project = @project";
		execStatus = sqlite3_prepare_v2(dbHandle, sql, -1, &res, 0);
		if(execStatus == SQLITE_OK) {
			int idx = sqlite3_bind_parameter_index(res, "@project");
			char* project = argv[2];
			sqlite3_bind_text(res, idx, project, strlen(project), NULL);
			
			int step = sqlite3_step(res);
			if(step == SQLITE_ROW) {
				pid = sqlite3_column_int(res, 0);	
			}
		}
	}

	sqlite3_finalize(res);

	/* insert the task with an optional project param  */
	const char* sql = pid == -1 ? 
		"INSERT INTO t.tasks(task)\
		VALUES (@task)"
		:
		"INSERT INTO t.tasks(task, project_id)\
		VALUES (@task, @pid)";
	execStatus = sqlite3_prepare_v2(dbHandle, sql, -1, &res, 0);
	if(execStatus == SQLITE_OK) {
		int taskIndex = sqlite3_bind_parameter_index(res, "@task");
		char* task = argv[1];
		sqlite3_bind_text(res, taskIndex, task, strlen(task), NULL);
		if(pid != -1) {
			int pidIndex = sqlite3_bind_parameter_index(res, "@pid");
			sqlite3_bind_int(res, pidIndex, pid);
		}
		execStatus = sqlite3_step(res);
		if(execStatus != SQLITE_DONE) {
			printf("%d \"%s\"\n", execStatus, sqlite3_errmsg(dbHandle));
		}
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(dbHandle));
	}

	sqlite3_close(dbHandle);
	return 0;
}
