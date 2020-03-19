#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  int fd;
  int *wd;

  fd = inotify_init();
  if (fd == -1)
  {
    perror("inotify_init");
    exit(EXIT_FAILURE);
  }

  wd = (int *)calloc(argc, sizeof(int));
  if (wd == NULL)
  {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++)
  {
    wd[i] = inotify_add_watch(fd, argv[i], IN_CREATE | IN_MOVED_FROM | IN_MOVED_TO | IN_MOVE_SELF | IN_MODIFY | IN_CLOSE_WRITE | IN_DELETE | IN_DELETE_SELF);
    if (wd[i] == -1)
    {
      fprintf(stderr, "Cannot watch '%s': %s\n",
              argv[i], strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  const struct inotify_event *event;
  ssize_t len;
  char *ptr;

  char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));

  for (;;)
  {
    printf("================================================\n");
    len = read(fd, buf, sizeof buf);
    if (len == -1 && errno != EAGAIN)
    {
      perror("read");
      exit(EXIT_FAILURE);
    }

    if (len <= 0)
      break;

    for (ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len)
    {

      event = (const struct inotify_event *)ptr;

      printf("%x ", event->mask);
      if (event->mask & IN_CREATE)
        printf("IN_CREATE: ");
      if (event->mask & IN_MOVED_FROM)
        printf("IN_MOVED_FROM: ");
      if (event->mask & IN_MOVED_TO)
        printf("IN_MOVED_TO: ");
      if (event->mask & IN_MOVE_SELF)
        printf("IN_MOVE_SELF: ");
      if (event->mask & IN_CLOSE_WRITE)
        printf("IN_CLOSE_WRITE: ");
      if (event->mask & IN_MODIFY)
        printf("IN_MODIFY: ");
      if (event->mask & IN_DELETE)
        printf("IN_DELETE: ");
      if (event->mask & IN_DELETE_SELF)
        printf("IN_DELETE_SELF: ");
      if (event->mask & IN_IGNORED)
        printf("IN_IGNORED: ");

      for (int i = 1; i < argc; ++i)
      {
        if (wd[i] == event->wd)
        {
          printf("%s/", argv[i]);
          break;
        }
      }

      if (event->len)
        printf("%s", event->name);

      if (event->mask & IN_ISDIR)
        printf(" [directory]\n");
      else
        printf(" [file]\n");
    }
  }
  close(fd);

  free(wd);
  exit(EXIT_SUCCESS);
}
