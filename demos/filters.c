#include <video.h>
#include <log.h>

int main(int argc, char *argv[])
{
	int err;

	err = video_init(640, 480);
	if(err)
		error("Failed to initialize video: %s", geterrmsg());

	exit(EXIT_SUCCESS);
}
