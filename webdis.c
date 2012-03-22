#include "server.h"
#include "conf.h"
#include "pool.h"
#include "worker.h"
#include "slog.h"

#include <string.h>
#include <syslog.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

int command_line_parameters(int argc, char *argv[]);
void usage();
static void signal_handler(int sig);

char *cfg_file = "webdis.json";

struct server *s;

int main(int argc, char *argv[]) {

#ifdef SIGHUP
	signal(SIGHUP,signal_handler);
#endif
	
	if ( command_line_parameters(argc, argv) != 0 ) {
		exit(EXIT_FAILURE);
	}

	s = server_new(cfg_file);

	server_start(s);
	
	return EXIT_SUCCESS;
}

int command_line_parameters(int argc, char *argv[]) {
	int c;
	FILE *file;
	
	opterr = 0;
	
	while ((c = getopt (argc, argv, "hf:")) != -1)
		switch (c)
		{
			case 'f':
				file = fopen(optarg, "r");
				if ( file != NULL ) {
					fclose(file);
					cfg_file = optarg;
				} else {
					return 1;
				}
				break;
			case 'h':
				usage();
			case '?':
				if (optopt == 'f')
					fprintf (stderr, "Option -%c requires the path to the webdis json configuration file.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,
						"Unknown option character `\\x%x'.\n",
						optopt);
				return 1;
			default:
				usage();
		}
	
	return 0;
}

void usage() {
	fprintf(stdout,"Webdis usage :\n");
	fprintf(stdout,"\t-f : path to the webdis json configuartion file (default : %s)\n",cfg_file);
	exit(EXIT_SUCCESS);
}

static void signal_handler(int sig)
{
	char *msg = "";
	
	switch(sig) {
		case SIGHUP:
			msg = "SIGHUP received... reload configuration";
			slog(s,WEBDIS_ERROR,msg,strlen(msg));
			s->cfg = conf_read(cfg_file);
			break;
		case SIGINT:
		case SIGTERM:
			break;
	}
}
