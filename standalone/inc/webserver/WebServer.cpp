#include "core/stdafx.h"
#include "WebServer.h"

#include "miniz/miniz.h"

#include <ifaddrs.h>

#ifdef __ANDROID__
#define LOG_TAG "WebServer"
#include "../../../src/utils/AndroidLog.h"
#define WEBLOGE ALOGE
#define WEBLOGI ALOGI
#else
#define WEBLOGE PLOGE.printf
#define WEBLOGI PLOGI.printf
#endif

static string myPath;

void WebServer::EventHandler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
   WebServer* webServer = (WebServer*)fn_data;

   if (ev == MG_EV_HTTP_MSG) {
      struct mg_http_message *hm = (struct mg_http_message *) ev_data;

      if (mg_http_match_uri(hm, "/files"))
         webServer->Files(c, hm);
      else if (mg_http_match_uri(hm, "/download"))
         webServer->Download(c, hm);
      else if (mg_http_match_uri(hm, "/upload"))
         webServer->Upload(c, hm);
      else if (mg_http_match_uri(hm, "/delete"))
         webServer->Delete(c, hm);
      else if (mg_http_match_uri(hm, "/folder"))
         webServer->Folder(c, hm);
      else if (mg_http_match_uri(hm, "/extract"))
         webServer->Extract(c, hm);
      else if (mg_http_match_uri(hm, "/activate"))
         webServer->Activate(c, hm);
      else if (mg_http_match_uri(hm, "/command"))
         webServer->Command(c, hm);
      else {
         string path = myPath + "assets" + PATH_SEPARATOR_CHAR + "vpx.html";

         struct mg_http_serve_opts opts = {};
         mg_http_serve_file(c, hm, path.c_str(), &opts);
      }
   }
}

WebServer::WebServer() 
{
   m_run = false;
   m_pThread = nullptr;  

#ifndef __ANDROID__
   m_szMyPrefPath = g_pvp->m_szMyPrefPath;
   myPath = g_pvp->m_szMyPath;
#endif
}

WebServer::~WebServer() 
{
   m_run = false;

   if (m_pThread) {
      m_pThread->join();
      delete m_pThread;
   }
}

#ifdef __ANDROID__
void WebServer::Init(string addr, int port, bool debug, string prefPath, string myServerPath)
{
   m_addr = addr;
   m_port = port;
   m_debug = debug;
   myPath = myServerPath;
   m_szMyPrefPath = prefPath;   
}
#endif

bool WebServer::Unzip(const char* pSource)
{
   mz_zip_archive zip_archive;
   memset(&zip_archive, 0, sizeof(zip_archive));

   mz_bool status = mz_zip_reader_init_file(&zip_archive, pSource, 0);
   if (!status) {
      WEBLOGE("Unable to unzip file: source=%s", pSource);
      return false;
   }

   bool success = true;

   int file_count = (int)mz_zip_reader_get_num_files(&zip_archive);

   for (int i = 0; i < file_count; i++) {
      mz_zip_archive_file_stat file_stat;
      if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
         success = false;

      string filename = file_stat.m_filename;
      if (filename.starts_with("__MACOSX"))
         continue;

      string path = std::filesystem::path(string(pSource)).parent_path().append(filename);
      if (mz_zip_reader_is_file_a_directory(&zip_archive, i))
         std::filesystem::create_directories(path);
      else {
         if (!mz_zip_reader_extract_to_file(&zip_archive, i, path.c_str(), 0)) {
            WEBLOGE("Unable to extract file: %s", path.c_str());
            success = false;
         }
      }
   }

   mz_zip_reader_end(&zip_archive);

   return success;
}

void WebServer::Files(struct mg_connection *c, struct mg_http_message* hm)
{
   char q[1024];
   mg_http_get_var(&hm->query, "q", q, sizeof(q));
   mg_remove_double_dots(q);

   WEBLOGI("Retrieving file list: q=%s", q);

   string path = m_szMyPrefPath + q;

   if (*q != '\0')
      path += PATH_SEPARATOR_CHAR;

   DIR* dir = opendir(path.c_str());

   if (!dir) {
      mg_http_reply(c, 400, "", "Bad request");
      return;
   }

   string json = "[ "s;

   int i = 0;

   struct dirent *entry;

   while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
         continue;

      if (i > 0)
         json += ", ";

      string file = path + entry->d_name;
      string ext;

      if (entry->d_type != DT_DIR)
         ext = extension_from_path(file);

      struct stat st;
      if (stat(file.c_str(), &st) == 0) {
         char* buf = mg_mprintf("{ %m: %m, %m: %m, %m: %s, %m: %lld }",
            MG_ESC("name"), MG_ESC(entry->d_name),
            MG_ESC("ext"), MG_ESC(ext.c_str()),
            MG_ESC("isDir"), entry->d_type == DT_DIR ? "true" : "false",
            MG_ESC("size"), (long long)st.st_size);

         json += buf;
         free(buf);

         i++;
      }
   }

   json += " ]";

   closedir(dir);

   mg_http_reply(c, 200, "", "%s", json.c_str());
}

void WebServer::Download(struct mg_connection *c, struct mg_http_message* hm)
{
   char q[1024];
   mg_http_get_var(&hm->query, "q", q, sizeof(q));
   mg_remove_double_dots(q);

   WEBLOGI("Downloading file: q=%s", q);

   if (*q == '\0') {
      mg_http_reply(c, 400, "", "Bad request");
      return;
   }

   string path = m_szMyPrefPath + q;

   struct mg_http_serve_opts opts = {};
   mg_http_serve_file(c, hm, path.c_str(), &opts);
}

void WebServer::Upload(struct mg_connection *c, struct mg_http_message* hm)
{
   char q[1024];
   mg_http_get_var(&hm->query, "q", q, sizeof(q));
   mg_remove_double_dots(q);

   WEBLOGI("Uploading file: q=%s", q);

   if (*q == '\0') {
      mg_http_reply(c, 400, "", "Bad request");
      return;
   }

   string path = m_szMyPrefPath + q;

   if (!mg_http_upload(c, hm, &mg_fs_posix, path.c_str(), 1024 * 1024 * 500)) {
// No need to reload VPinballX.ini when running in Android
// It will be loaded when a table is launched 
#ifndef __ANDROID__ 
      if (!strncmp(q, "VPinballX.ini", sizeof(q)))
         g_pvp->m_settings.LoadFromFile(path, false);
#endif
   }
}

void WebServer::Delete(struct mg_connection *c, struct mg_http_message* hm)
{
   char q[1024];
   mg_http_get_var(&hm->query, "q", q, sizeof(q));
   mg_remove_double_dots(q);

   if (*q == '\0') {
      mg_http_reply(c, 400, "", "Bad request");
      return;
   }

   string path = m_szMyPrefPath + q;

   if (std::filesystem::is_regular_file(path)) {
      if (std::filesystem::remove(path.c_str()))
         mg_http_reply(c, 200, "", "OK");
      else
         mg_http_reply(c, 500, "", "Server error");
   }
   else if (std::filesystem::is_directory(path)) {
      if (std::filesystem::remove_all(path) != 0)
         mg_http_reply(c, 200, "", "OK");
      else
         mg_http_reply(c, 500, "", "Server error");
   }
   else
      mg_http_reply(c, 400, "", "Bad request");
}

void WebServer::Folder(struct mg_connection *c, struct mg_http_message* hm)
{
   char q[1024];
   mg_http_get_var(&hm->query, "q", q, sizeof(q));
   mg_remove_double_dots(q);

   if (*q == '\0') {
      mg_http_reply(c, 400, "", "Bad request");
      return;
   }

   string path = m_szMyPrefPath + q;

   if (std::filesystem::create_directory(path))
      mg_http_reply(c, 200, "", "OK");
   else
      mg_http_reply(c, 500, "", "Server error");
}

void WebServer::Extract(struct mg_connection *c, struct mg_http_message* hm)
{
   char q[1024];
   mg_http_get_var(&hm->query, "q", q, sizeof(q));
   mg_remove_double_dots(q);

   if (*q == '\0') {
      mg_http_reply(c, 400, "", "Bad request");
      return;
   }

   string path = m_szMyPrefPath + q;

   if (std::filesystem::is_regular_file(path)) {
      if (extension_from_path(path) == "zip") {
         if (Unzip(path.c_str())) {
            WEBLOGI("File unzipped: q=%s", path.c_str());
            mg_http_reply(c, 200, "", "OK");
         }
         else
            mg_http_reply(c, 500, "", "Server error");
      }
      else
         mg_http_reply(c, 400, "", "Bad request");
   }
   else
      mg_http_reply(c, 400, "", "Bad request");
}

void WebServer::Activate(struct mg_connection *c, struct mg_http_message* hm)
{
#ifndef __ANDROID__   
   char q[1024];
   mg_http_get_var(&hm->query, "q", q, sizeof(q));
   mg_remove_double_dots(q);

   WEBLOGI("Activating table: q=%s", q);

   if (*q == '\0') {
      mg_http_reply(c, 400, "", "Bad request");
      return;
   }

   g_pvp->m_settings.SaveValue(Settings::Standalone, "LaunchTable"s, q);

   mg_http_reply(c, 200, "", "OK");
#else
   mg_http_reply(c, 500, "", "Not Supported");
#endif
}

void WebServer::Command(struct mg_connection *c, struct mg_http_message* hm)
{
   char cmd[1024];
   mg_http_get_var(&hm->query, "cmd", cmd, sizeof(cmd));

   if (*cmd == '\0') {
      mg_http_reply(c, 400, "", "Bad request");
      return;
   }

   if (!strncmp(cmd, "fps", sizeof(cmd))) {
      g_pplayer->m_liveUI->ToggleFPS();

      mg_http_reply(c, 200, "", "OK");
   }
   else if (!strncmp(cmd, "shutdown", sizeof(cmd))) {
#ifndef __ANDROID__
      g_pvp->GetActiveTable()->QuitPlayer(Player::CS_CLOSE_APP);

      mg_http_reply(c, 200, "", "OK");
#else
      mg_http_reply(c, 500, "", "Not Supported");
#endif
   }
   else
      mg_http_reply(c, 400, "", "Bad request");
}

string WebServer::GetUrl()
{
   return m_run ? m_url : string();
}

void WebServer::Start()
{
   if (m_run) {
      WEBLOGE("Web server already running");
      return;
   }

#ifndef __ANDROID__
   m_debug = g_pvp->m_settings.LoadValueWithDefault(Settings::Standalone, "WebServerDebug"s, false);
   m_addr = g_pvp->m_settings.LoadValueWithDefault(Settings::Standalone, "WebServerAddr"s, "0.0.0.0"s);
   m_port = g_pvp->m_settings.LoadValueWithDefault(Settings::Standalone, "WebServerPort"s, 2112);
#endif

   if (m_debug) {
      mg_log_set(MG_LL_DEBUG);
      WEBLOGI("Web server debug enabled");
   }

   string bindUrl = "http://" + addr + ':' + std::to_string(port);

   PLOGI.printf("Starting web server at %s", bindUrl.c_str());

   mg_mgr_init(&m_mgr);

   if (mg_http_listen(&m_mgr, bindUrl.c_str(), &WebServer::EventHandler, this)) {
      m_run = true;

      WEBLOGI("Web server started");

      string ip = GetIPAddress();

      if (!ip.empty()) {
         m_url = "http://" + ip + ':' + std::to_string(port);

         WEBLOGI("To access the web server, in a browser go to: %s", m_url.c_str());
      }
      else
         m_url.clear();

      m_pThread = new std::thread([this]() {
         while (m_run)
            mg_mgr_poll(&m_mgr, 1000);

         mg_mgr_free(&m_mgr);
         m_url.clear();

         WEBLOGI("Web server closed");
      });
   }
   else {
      WEBLOGE("Unable to start web server");
   }
}

void WebServer::Stop()
{
   if (!m_run) {
      WEBLOGE("Web server is not running");
      return;
   }

   m_run = false;

   if (m_pThread) {
      m_pThread->join();
      delete m_pThread;

      m_pThread = nullptr;
   }
}

bool WebServer::IsRunning()
{
   return m_run;
}

string WebServer::GetIPAddress()
{
   struct ifaddrs *ifaddr;
   struct ifaddrs *ifa;

   if (getifaddrs(&ifaddr) == -1)
      return string();

   for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr == nullptr)
         continue;

      int family = ifa->ifa_addr->sa_family;

      if (family == AF_INET) {
         if (!strcmp(ifa->ifa_name, "wlan0") || !strcmp(ifa->ifa_name, "eth0") || !strcmp(ifa->ifa_name, "en0") || !strcmp(ifa->ifa_name, "en1")) {
            char host[NI_MAXHOST];
            int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            freeifaddrs(ifaddr);
            if (s != 0)
               return string();
            else
               return host;
         }
      }
   }

   freeifaddrs(ifaddr);

   return string();
}
