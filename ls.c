#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <stdbool.h>

extern int errno ;


void blue()
{
    printf("\033[0;34m");
}
void red() 
{
  printf("\033[1;31m");
}

void green()
{
    printf("\033[0;32m");
}

void pink()
{
    printf("\033[0;35m");
}
void reset() // resets color to default 
{
  printf("\033[0m");
}
void reverse_video() // displays in reverse video, my setup shows black font white foreground
{
   printf("\033[7m");
}

void set_color(char* fname) // sets color depending on file type
{
    
  struct stat info;
  char time[50];
  int x = lstat (fname, &info);
  mode_t mode;    

  int rv = lstat(fname, &info);
  if (rv == -1){
      perror("stat failed");
      exit(1);
  }
    
  if (stat(fname, &info) == 0 && S_ISDIR(info.st_mode))
  {
      blue();
  }
  if (stat(fname, &info) == 0 && S_ISCHR(info.st_mode))
  {
      reverse_video();
  }
  if (stat(fname, &info) == 0 && S_ISBLK(info.st_mode))
  {
      reverse_video();
  }

  if (S_ISLNK(info.st_mode))
  {
      pink();
  }
}

bool is_dir(char *fname) // tell us whether a path is a directory
{
  struct stat info;
  int x = lstat (fname, &info);
  char time[50];

  mode_t mode;   
  int rv = lstat(fname, &info);
  if (rv == -1)
  {
    perror("stat failed");
    exit(1);
  } 
  if (stat(fname, &info) == 0 && S_ISDIR(info.st_mode))
  {  return true; }

  return false;
}
void show_stat_info(char *fname, char *filename_s){
    
   
  struct stat info;
  int x = lstat (fname, &info);
  char time[50];
 
  mode_t mode;    

  int rv = lstat(fname, &info);

    if (rv == -1)
  {
      perror("stat failed");
      exit(1);
  }
    
  if (stat(fname, &info) == 0 && S_ISREG(info.st_mode))
  {
      printf("-");
  }
      
  if (stat(fname, &info) == 0 && S_ISDIR(info.st_mode))
  {   
    
      printf("d");
      
  }
  if (stat(fname, &info) == 0 && S_ISCHR(info.st_mode))
  {   
      
      printf("c");
      
  }
  if (stat(fname, &info) == 0 && S_ISBLK(info.st_mode))
  {    
      printf("b");
  }
  if (stat(fname, &info) == 0 && S_ISFIFO(info.st_mode))
  {
      printf("p");
  }
  if (S_ISLNK(info.st_mode))
  {   
      printf("l");
      // buf.st_mode & S_IFMT
  }
    
  int mde = info.st_mode; 
  char str[10];
  strcpy(str, "---------");
//owner  permissions
  if((mde & 0000400) == 0000400) str[0] = 'r';
  if((mde & 0000200) == 0000200) str[1] = 'w';
  if((mde & 0000100) == 0000100) str[2] = 'x';
  //group permissions
  if((mde & 0000040) == 0000040) str[3] = 'r';
  if((mde & 0000020) == 0000020) str[4] = 'w';
  if((mde & 0000010) == 0000010) str[5] = 'x';
  //others  permissions
  if((mde & 0000004) == 0000004) str[6] = 'r';
  if((mde & 0000002) == 0000002) str[7] = 'w';
  if((mde & 0000001) == 0000001) str[8] = 'x';
  //special  permissions
  if((mde & 0004000) == 0004000) str[2] = 's';
  if((mde & 0002000) == 0002000) str[5] = 's';
  if((mde & 0001000) == 0001000) str[8] = 't';
  printf("%s ", str);

  printf("%ld ", info.st_nlink);

  struct passwd *pw = getpwuid(info.st_uid);
  struct group  *gr = getgrgid(info.st_gid);

  if (pw != 0) 
    printf("%s ", pw->pw_name); // user 

  if (gr != 0) 
    printf("%s ", gr->gr_name); // group 

  printf("%5ld ", info.st_size);

  strftime(time, 50, "%b %d %H:%M", localtime(&info.st_mtime));
  printf ("%s ", time);
 
  set_color(fname);
  printf("%-3s\n", filename_s );
  reset(); 

//    printf("user: %d\n", info.st_uid);
//    printf("group: %d\n", info.st_gid); 
}



void print_all_files_in_dir(char* file_path) // prints all files/dirs in a folder
{                                          // in a column 
  char array[500][500], temp[500];
  DIR *d;
  int num_files = 0;
  struct dirent *dir;
  d = opendir(file_path);
  if (d)
  {
      while ((dir = readdir(d)) != NULL)
      {
          if (errno != 0 && d == NULL) 
          { printf("Readir failed \n"); return;}		
          printf("%s\n", dir->d_name);
          strcpy(array[num_files], dir->d_name);
          num_files++;
      }
      printf("%d\n", num_files);
      closedir(d);
  }
    
  for (int i = 0; i < num_files; ++i) {
    for (int j = i + 1; j < num_files; ++j) {

        // swapping strings if they are not in the lexicographical order
        if (strcmp(array[i], array[j]) > 0) {
          strcpy(temp, array[i]);
          strcpy(array[i], array[j]);
          strcpy(array[j], temp);
        }
    }
  }
    
  for (int counter = 0; counter < num_files; counter++)
  {
      printf("%s\n", array[counter]);
  }
}


void print_all_files_in_dir_row(char* file_path) // prints all files/dirs in a folder
{                                          // in a row
  char array[500][500], temp[500];
  DIR *d;
  int num_files = 0;
  struct dirent *dir;
  d = opendir(file_path);
  if (d)
  {
  while ((dir = readdir(d)) != NULL)
  {
      if (errno != 0 && d == NULL) 
      { printf("Readir failed \n"); return;}		
      strcpy(array[num_files], dir->d_name);
      num_files++;
  }
  closedir(d);
  }
    
  for (int i = 0; i < num_files; ++i) {
    for (int j = i + 1; j < num_files; ++j) 
    {

        // swapping strings if they are not in the lexicographical order
        if (strcmp(array[i], array[j]) > 0) {
          strcpy(temp, array[i]);
          strcpy(array[i], array[j]);
          strcpy(array[j], temp);
        }
    }
  }

  for (int counter = 0; counter < num_files; counter++)
  {   
      if (array[counter][0] != '.')
      printf("%s ", array[counter]);
  }
  printf("\n");
}


//function to print file/directory data with ls option -l

void ls_a(char* file_path) // ls -a show hidden files
{
  char array[500][500], temp[500];
  DIR *d;
  int num_files = 0;
  struct dirent *dir;
  d = opendir(file_path);
  if (d)
  {
      while ((dir = readdir(d)) != NULL)
      {
          if (errno != 0 && d == NULL) 
          { printf("Readir failed \n"); return;}		
          //printf("%s\n", dir->d_name);
          strcpy(array[num_files], dir->d_name);
          num_files++;
      }
      //printf("%d\n", num_files);
      closedir(d);
  }
    
  for (int outer = 0; outer < num_files; ++outer) { // lex sort
    for (int inner = outer + 1; inner < num_files; ++inner) {

        
        if (strcmp(array[outer], array[inner]) > 0) {
          strcpy(temp, array[outer]);
          strcpy(array[outer], array[inner]);
          strcpy(array[inner], temp);
        }
    }
  }
    
  for (int counter = 0; counter < num_files; counter++)
  {
      //if (array[counter][0] != "."
      printf("%s ", array[counter]) ;
  }
  printf("\n");
}


void ls_r(char* file_path) // ls -R
{
// chdir(fi);
  char *filename_s;
  char array[500][500], temp[500];
  DIR *d;
  int num_files = 0;
  struct dirent *dir;
  char path[1000];
  
  char buf[1000];
  
  d = opendir(file_path);
  if (d)
  {
      while ((dir = readdir(d)) != NULL)
      {
          if (errno != 0 && d == NULL) 
          { printf("Readir failed \n"); return;}		
          //printf("%s\n", dir->d_name);
          
          strcpy(array[num_files], dir->d_name);
          num_files++;
      }
      //printf("%d\n", num_files);
      closedir(d);
  }

  for (int outer = 0; outer < num_files; ++outer) { // lex sort
    for (int inner = outer + 1; inner < num_files; ++inner) {

        
        if (strcmp(array[outer], array[inner]) > 0) {
          strcpy(temp, array[outer]);
          strcpy(array[outer], array[inner]);
          strcpy(array[inner], temp);
        }
    }
  }  

    
  for (int counter = 0; counter < num_files; counter++)
  {
    strcpy(buf, "");
    sprintf(buf, "%s/%s", file_path, array[counter]);
    if (is_dir(buf) != true &&  array[counter][0] != '.' )
    { if(strcmp(array[counter], ".") == 0) {printf(".:\n");}
    set_color(buf); printf("%s ", array[counter] ); reset();}
  }
  printf("\n\n");

  for (int counter = 0; counter < num_files; counter++)
  {
      if (array[counter][0] != '.')
        {
        
        filename_s = array[counter];
        sprintf(buf, "%s/%s", file_path, array[counter]);
        if ( is_dir(buf) )
        {
        strcpy(path, "");
        strcat(path, "./");
        strcat(path, filename_s);      
        printf("%s:\n",path); 
        
        print_all_files_in_dir_row(filename_s);
      
        printf("\n"); }

        //show_stat_info(buf, filename_s);
        //reset();
        }
  }
    // printf("\n");


}

void ls_l(char* file_path) // does long listing for of all files and dirs in a folder
{
// chdir(fi);
 
  char *filename_s;
  char array[500][500], temp[500];
  DIR *d;
  int num_files = 0;
  struct dirent *dir;
  char path[500];
  //sprintf(path, "%s/%s", file_path, dir->d_name);
  char buf[1000];
    
  d = opendir(file_path);
  if (d)
  {
      while ((dir = readdir(d)) != NULL)
      {
          if (errno != 0 && d == NULL) 
          { printf("Readir failed \n"); return;}		
          //printf("%s\n", dir->d_name);
          
          strcpy(array[num_files], dir->d_name);
          num_files++;
      }
      //printf("%d\n", num_files);
      closedir(d);
  }
    
  for (int outer = 0; outer < num_files; ++outer) { // lex sort
    for (int inner = outer + 1; inner < num_files; ++inner) {

        
        if (strcmp(array[outer], array[inner]) > 0) {
          strcpy(temp, array[outer]);
          strcpy(array[outer], array[inner]);
          strcpy(array[inner], temp);
        }
    }
  }

  for (int counter = 0; counter < num_files; counter++)
  {
      if (array[counter][0] != '.')
        {
        
        filename_s = array[counter];
        sprintf(buf, "%s/%s", file_path, array[counter]);
        show_stat_info(buf, filename_s);
        //reset();
        }
  }
  printf("\n");


}

void total(char  *filename) // calcualates the total part of ls -l, first line of ls -l
{
  char fir[200] = "ls -s ";
  char fol[100] = "";
  strcpy(fol, filename);
  
  strcat(fir, fol);
  strcat(fir, "| head -1|grep -o '[0-9]*'");
  printf("total ");
  system(fir);
  printf("\n");
}
int main(int argc , char *argv[])
{

  
  //long listing or ls -l
  if (argc == 1) // show subdirectories and files in a folder in lex sorting
  {
   
      ls_l(".");
  }

  else if (argc >= 2)
  {
    for (int counter = 1; counter < argc; counter++)
    {   printf("%s:\n", argv[counter]);
        total(argv[counter]);
        ls_l( argv[counter]);
        
    } 
  } 


  //ls -R recursive 
  // if (argc == 1) // show subdirectories and files in a folder in lex sorting
  // {
  //     ls_r(".");
  // }

  // else if (argc >= 2)
  // {
  //   for (int counter = 1; counter < argc; counter++)
  //   {   printf("%s:\n", argv[counter]);
  //       ls_r( argv[counter]);
  //   } 
  // }
    return(0);
}




