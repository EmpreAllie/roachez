#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <string>
#include <ctime>
#include <cmath>

#define BUFSIZE 4096

#define PRINT() \
   if (!f) return; \
   time_t tm = time(NULL); \
   struct tm * now = localtime(&tm); \
   va_list ap; \
   va_start(ap, format); \
   Logfoo(now, status, format, ap); \
   va_end(ap);


extern FILE * f;

double normalizeAngle(double direction)
{
    if (direction < 0) direction = 360 + direction;
    else if (direction >= 360) direction = fmod(direction, 360.0);
    return direction;
}

double degToRad(double Dir)
{
   return (fmod(Dir, 90) * M_PI / 180);
}

double radToDeg(double DirInRad)
{
   return DirInRad * (180 / M_PI);
}

int random(int Min, int Max)
{
   if (Min > Max) std::swap(Min, Max);
   return rand() % (Max - Min + 1) + Min;
}

void Logfoo(struct tm * tm, char * status, const char * format, va_list ap) {
   char buf[BUFSIZE] = {0};

   strftime(buf + strlen(buf), sizeof buf, "%H:%M:%S  ", tm);
//   sprintf(buf + strlen(buf), "%02d:%02d:%02d  ", tm->tm_hour, tm->tm_min, tm->tm_sec);
   sprintf(buf + strlen(buf), "%s  ", status);
   vsprintf(buf + strlen(buf), format, ap);
   fprintf(f, "%s\n", buf);
   fflush(f);
}

void Logi(const char * format, ...)
{
   char status[] = "INFO    :";
   PRINT();
}

void Logw(const char * format, ...)
{
   char status[] = "WARNING :";
   PRINT();
}

void Loge(const char * format, ...)
{
   char status[] = "ERROR   :";
   PRINT();
}

void Log(const char * format, ...)
{
   va_list ap;
   va_start(ap, format);
   if (f)
   {
      vfprintf(f, format, ap);
      fprintf(f, "\n");
      fflush(f);
   }
   va_end(ap);
}