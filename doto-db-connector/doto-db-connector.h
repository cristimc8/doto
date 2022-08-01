#ifndef HEADER_DOTO-DB-CONNECTOR
#define HEADER_DOTO-DB-CONNECTOR

enum PURPOSE {
    DISPLAY = 0,
    SAVE_TASK = 1,
	SAVE_PROJECT = 2,
	INVALID = 3
};

/**
  * Opens the db
  * returns -1 if anything does not work
  */
int openDb();
/**
  * Returns the project's id from the provided name
  * or -1 if the project is not found.
  */
int getProjectIdByName(char*);
/**
  * Saves a task with an optional associated project
  * Used with `getProjectIdByName` to provide the second
  * id argument.
  */
void saveTask(char*, int);
/**
  * Saves a project with a provided name.
  * If a project with the same name exists, a message is
  * showed and status code returned is 1.
  */
int saveProject(char*);
/**
  * Gets the purpose of the connector, whether it's used
  * to display some data or save some tasks.
  */
PURPOSE getConnectorPurpose(int, char*[]);
/**
  * Function used when the connector's purpose is `display`.
  */
int displayForArgs(int, char*[]);
/**
  * Function used when the connector's purpose is `save`.
  * For a save / update the call will always be like:
  * ./doto-db-connector save {task} +-{project}
  */
int saveTaskForArgs(int, char*[]);

#endif
