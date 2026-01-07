#define TEST_WRITE


double now() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

void write_file(const char *filename, unsigned int block_size, unsigned int block_count) 
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) 
    {
        perror("Error opening file for writing");
        return;
    }
    char *buffer = malloc(block_size);
    if (!buffer) 
    {
        perror("Memory allocation failed for write buffer");
        close(fd);
        return;
    }

    memset(buffer, 'A', block_size);  
    double start = now();

    for (int i = 0; i < block_count; ++i) 
    {
        if (write(fd, buffer, block_size) == -1) 
        {
            perror("Error writing to file");
            break;
        }
    }

    double end = now();
    double time_seconds = end - start;
    double total = block_size * (double)block_count;
    double performance = total / ((1024.0 * 1024.0) * time_seconds);

    printf("Write completed in %f seconds\n", time_seconds);
    printf("Performance: %f MiB/s\n", performance);

    free(buffer);
    close(fd);
}

void read_file(const char *filename, unsigned int block_size, unsigned int block_count) 
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1) 
    {
        perror("Error opening file for reading");
        return;
    }

    struct stat st;
    if (stat(filename, &st) == -1) 
    {
        perror("Error getting file size");
        close(fd);
        return;
    }

    if (st.st_size < (off_t)(block_size * block_count)) 
    {
        printf("Error: File size is insufficient for the requested read operation\n");
        close(fd);
        return;
    }

    unsigned int *buffer = malloc(block_size);
    if (!buffer) 
    {
        perror("Memory allocation failed for read buffer");
        close(fd);
        return;
    }

    double start = now();
    unsigned int xorValue = 0;

    for (int i = 0; i < block_count; ++i) 
    {
        int bytes_read = read(fd, buffer, block_size);
        if (bytes_read == -1) 
        {
            perror("Error reading from file");
            break;
        }
        xorValue ^= xorbuf(buffer, bytes_read / sizeof(unsigned int));
    }

    double end = now();
    double time_seconds = end - start;
    double totalBytesRead = block_size * (double)block_count;
    double performance = totalBytesRead / ((1024.0 * 1024.0) * time_seconds);

    printf("Read completed in %f seconds\n", time_seconds);
    printf("Performance: %f MiB/s\n", performance);
    printf("XOR Value: %08x\n", xorValue);

    free(buffer);
    close(fd);
}

static void do_fs_test(const char *sda, const char *mnt, const char *file)
{
  rtems_status_code sc;
  int rv;

  static const char test_string[] =
      "The quick brown fox jumps over the lazy dog\n";

  int count;
  int SIZE = strlen(test_string);
  int total_size = 1024*4;
  int fd;
  ssize_t num_bytes;

#ifdef TEST_WRITE
  // test write
  count = total_size;
  fd = open(file, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
  rtems_test_assert(fd >= 0);
  while (count > 0)
  {
    num_bytes = write(fd, test_string, SIZE);
    // printf("write num_bytes %d\n", num_bytes);
    rtems_test_assert(SIZE == num_bytes);
    count -= num_bytes;
  }
  rv = close(fd);
  rtems_test_assert(0 == rv);
#endif

  // test read
  char *cluster_buf = calloc(SIZE + 1, 1);

  count = total_size;
  fd = open(file, O_RDONLY, S_IRWXU | S_IRWXG | S_IRWXO);
  rtems_test_assert(fd >= 0);
  while (count > 0)
  {
    num_bytes = read(fd, cluster_buf, SIZE);
    rtems_test_assert(SIZE == num_bytes);
    count -= num_bytes;
    rtems_test_assert(0 == strncmp(test_string, cluster_buf, num_bytes));
  }
  rv = close(fd);
  rtems_test_assert(0 == rv);

  free(cluster_buf);

#if 0 // large file benchmark
  int order = 1;
  u64 STEP = 1;
  for (STEP = 1; STEP <= (128UL * 1024 * 1024); STEP <<= 1)
  {
    int fd;
    size_t num_bytes;
    uint8_t *cluster_buf = malloc(STEP); // XXX: must use heap!!!
    memset(cluster_buf, 0xAB, STEP);
    fd = open(file, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    rtems_test_assert(fd >= 0);
    num_bytes = write(fd, cluster_buf, STEP);
    printf("write num_bytes %d\n", num_bytes);
    rtems_test_assert(STEP == num_bytes);
    rv = close(fd);
    rtems_test_assert(0 == rv);

    uint8_t *cluster_buf2 = malloc(STEP);
    fd = open(file, O_RDONLY);
    rtems_test_assert(fd >= 0);
    num_bytes = read(fd, cluster_buf2, STEP);
    printf("read num_bytes %d\n", num_bytes);
    rtems_test_assert(STEP == num_bytes);
    printf("[%d] strncmp %d\n\n", order++, 0 == strncmp(cluster_buf, cluster_buf2, STEP));
    rv = close(fd);
    rtems_test_assert(0 == rv);
    free(cluster_buf);
    free(cluster_buf2);
  }
#endif

  printf("=== test file done and succeed ===\n");

}

static void fs_test(const char *sda, const char *mnt, const char *file)
{
  int rv;

#ifdef TEST_WRITE
  static const msdos_format_request_param_t rqdata = {
      .sync_device = true,
      .quick_format = true,
  };
  rv = msdos_format(sda, &rqdata); // should avoid re-formating after writing
  rtems_test_assert(rv == 0);
#endif

  rtems_dosfs_mount_options mount_options;
  memset(&mount_options, 0, sizeof(mount_options));
  mount_options.converter = rtems_dosfs_create_utf8_converter("CP850");

  rv = mount_and_make_target_path(
      sda,
      mnt,
      RTEMS_FILESYSTEM_TYPE_DOSFS,
      RTEMS_FILESYSTEM_READ_WRITE,
      &mount_options);
  rtems_test_assert(rv == 0);

  printf("=== start fs test === \n");

  do_fs_test(sda, mnt, file);

  rv = unmount(mnt);
  rtems_test_assert(rv == 0);
}
