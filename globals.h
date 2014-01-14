/* */
//==============================================================
// "globals"
//==============================================================
uint8_t      gAPN[32] = "APN";                            // APN from your service provider
uint8_t   gServer[16] = "ftpServer";                      // URL of your FTP server
uint8_t gUserName[16] = "username";                       // username for ftp connect
uint8_t gPassword[16] = "password";                       // password for ftp connect
uint8_t  gFTPpath[16] = "/ftpPath/";                      // path to location of ftp files
uint8_t     gWxId[16] = "wxID";                           // id of your weather station
uint8_t    gMagic[ 8] = "wxftp";                          // to identify valid records
uint8_t   gFtpSql[48] = "http://www..../wxftp2sql.php";   // URL of php script to read ftp and write to database
uint8_t  gTimeURL[48] = "http://www..../ts.php";          // URL that returns Unix epoch
/* */


