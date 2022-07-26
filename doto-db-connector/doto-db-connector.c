#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if(argc == 1) {
		printf("We need a task to store, along with an optional project, but the connector recieved 0 arguments.");
		return 1;
	}

	sqlite3 *dbHandle;
	sqlite3_stmt *res;
	char *errMsg = 0;
	int execStatus;

	if((execStatus = sqlite3_open("doto-db.sqlite", &dbHandle)) != SQLITE_OK) {
		fprintf(stderr, "An error occured: %s\n", sqlite3_errmsg(dbHandle));
		sqlite3_close(dbHandle);

		return 1;
	}

	/* db is opened at this point */

	char *sql = "INSERT INTO tasks(task, project_id) VALUES	(@task, @pid)";
	execStatus = sqlite3_prepare_v2(dbHandle, sql, -1, &res, 0);


	sqlite3_close(dbHandle);
	return 0;
}
