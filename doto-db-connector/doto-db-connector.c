#include "sqlite3.h"
#include "doto-db-connector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* dbName = "doto-db.sqlite";
sqlite3 *dbHandle;
sqlite3_stmt *res;
char* errMsg = 0;
int execStatus;

char *removeEnd(char *str, char c)
{
   char *last_pos = strrchr(str, c);

   if (last_pos != NULL) {
      *last_pos = '\0';
      return last_pos + 1; /* pointer to the removed part of the string */
   }

   return NULL;  /* c wasn't found in the string */
}

void createDbName(char* dbPath, char *argv[]) {
	strcpy(dbPath, argv[0]);
	removeEnd(dbPath, '/');
	strcat(strcat(dbPath, "/"), dbName);
}

int main(int argc, char* argv[]) {
	char* dbPath = (char*) malloc(strlen(argv[0]) + strlen(dbName) + 2);
	createDbName(dbPath, argv);

	PURPOSE connectorPurpose = getConnectorPurpose(argc, argv);
	int code = 0;

	if(openDb(dbPath) != 0) {
		return 1;
	}

	switch(connectorPurpose) {
		case DISPLAY:
			code = displayForArgs(argc, argv);
			break;
		case SAVE_TASK:
			code = saveTaskForArgs(argc, argv);
			break;
		case SAVE_PROJECT:
			code = saveProject(argv[2]);
			break;
		case CHECK_PROJECT:
			fprintf(stdout, "%d", getProjectIdByName(argv[2]));
			code = 0;
			break;
		default:
			fprintf(stderr, "Invalid purpose detected\n");
			return 1;
	}	
	sqlite3_close(dbHandle);
	return code;
}

int openDb(char* dbPath) {
	if((execStatus = sqlite3_open(dbPath, &dbHandle)) != SQLITE_OK) {
		fprintf(stderr, "An error occured: %s\n", sqlite3_errmsg(dbHandle));
		sqlite3_close(dbHandle);
		
		free(dbPath);
		return 1;
	}
	free(dbPath);
	return 0;
}

int saveTaskForArgs(int argc, char* argv[]) {
	int pid = -1;
	/* get the project id if project was supplied  */
	if(argc == 4) {
		pid = getProjectIdByName(argv[3]);
	}

	/* insert the task with an optional project param  */
	saveTask(argv[2], pid);
	return 0;
}

int displayForArgs(int argc, char* argv[]) {
	int pid = -1;
	if(argc == 3) {
		pid = getProjectIdByName(argv[2]);
	}

	const char* sql = argc == 2 ?
		"SELECT * FROM tasks\
		WHERE project_id is null"
		:
		"SELECT * FROM tasks\
		WHERE project_id = @pid";

	execStatus = sqlite3_prepare_v2(dbHandle, sql, -1, &res, 0);
	if(execStatus == SQLITE_OK) {
		int idx = sqlite3_bind_parameter_index(res, "@pid");
		sqlite3_bind_int(res, idx, pid);
	
		int step = sqlite3_step(res);
		if(step == SQLITE_ROW) {
			pid = sqlite3_column_int(res, 0);	
		}
	}
	execStatus = sqlite3_step(res);
	if(execStatus != SQLITE_DONE) {
		printf("%d \"%s\"\n", execStatus, sqlite3_errmsg(dbHandle));
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(dbHandle));
	}

	sqlite3_finalize(res);
	return pid;

}

PURPOSE getConnectorPurpose(int argc, char* argv[]) {

	if(strcmp(argv[1], "display") == 0) {
		return DISPLAY;
	}
	else if(strcmp(argv[1], "saveTask") == 0) {
		return SAVE_TASK;
	}
	else if(strcmp(argv[1], "saveProject") == 0) {
		return SAVE_PROJECT;
	}
	else if(strcmp(argv[1], "checkProject") == 0) {
		return CHECK_PROJECT;
	}
	return INVALID;
}

int saveProject(char* project) {
	const char* sql = "INSERT INTO projects(project)"
						"VALUES(@projectName)";
	execStatus = sqlite3_prepare_v2(dbHandle, sql, -1, &res, 0);
	if(execStatus == SQLITE_OK) {
		int idx = sqlite3_bind_parameter_index(res, "@projectName");
		sqlite3_bind_text(res, idx, project, strlen(project), NULL);
	
		execStatus = sqlite3_step(res);
		if(execStatus != SQLITE_DONE) {
			printf("%d \"%s\"\n", execStatus, sqlite3_errmsg(dbHandle));
		}
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(dbHandle));
		sqlite3_finalize(res);
		return 1; 
	}
	sqlite3_finalize(res);
	return 0;
}

int getProjectIdByName(char* project) {
	int pid = -1;
	const char* sql = "SELECT id FROM "
						"projects WHERE project = @project";
	execStatus = sqlite3_prepare_v2(dbHandle, sql, -1, &res, 0);
	if(execStatus == SQLITE_OK) {
		int idx = sqlite3_bind_parameter_index(res, "@project");
		sqlite3_bind_text(res, idx, project, strlen(project), NULL);
	
		int step = sqlite3_step(res);
		if(step == SQLITE_ROW) {
			pid = sqlite3_column_int(res, 0);	
		}
		execStatus = sqlite3_step(res);
		if(execStatus != SQLITE_DONE) {
			printf("%d \"%s\"\n", execStatus, sqlite3_errmsg(dbHandle));
		} 
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(dbHandle));
	}

	sqlite3_finalize(res);
	return pid;
}

void saveTask(char* task, int pid) {
	const char* sql = pid == -1 ? 
		"INSERT INTO tasks(task) \
		VALUES (@task)"
		:
		"INSERT INTO tasks(task, project_id) \
		VALUES (@task, @pid)";
	execStatus = sqlite3_prepare_v2(dbHandle, sql, -1, &res, 0);
	if(execStatus == SQLITE_OK) {
		int taskIndex = sqlite3_bind_parameter_index(res, "@task");
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
	sqlite3_finalize(res);
}

