#include <qtgeography.h>

N::EarthLocation:: EarthLocation (void)
                 : location      (0   )
{
}

N::EarthLocation:: EarthLocation(double longitude,double latitude)
{
  setLocation ( longitude , latitude ) ;
}

N::EarthLocation::~EarthLocation(void)
{
}

unsigned int N::EarthLocation::setLongitude(double longitude)
{
  unsigned int       longi = getLongitude ( longitude ) ;
  unsigned long long L     = location                   ;
  L       &= 0xFFFFFFFF00000000LL                       ;
  L       |= longi                                      ;
  location = L                                          ;
  return longi                                          ;
}

unsigned int N::EarthLocation::setLatitude(double latitude)
{
  unsigned int       lati = getLatitude ( latitude ) ;
  unsigned long long L    = lati                     ;
  L      <<= 32                                      ;
  L       |= ( location & 0x00000000FFFFFFFFLL )     ;
  location = L                                       ;
  return lati                                        ;
}

unsigned int N::EarthLocation::getLongitude(QString longitude)
{
  unsigned int longi = 0;
  int d,l,m,s,v;
  char XLINE[2048];
  char D[64];
  D[0] = 'E';
  ::strcpy(XLINE,(const char *)longitude.toUtf8().constData());
  ::sscanf(XLINE,"%s %d %d' %d.%d",D,&l,&m,&s,&v);
  if (D[0]=='E') d = 1; else d = 0;
  if (l>=180) l = 180;
  if (d==1) longi |= 0x1;
  longi <<= 8;
  longi  |= l;
  longi <<= 6;
  longi  |= m;
  longi <<= 6;
  longi  |= s;
  longi <<= 11;
  longi  |= v;
  return longi;
}

unsigned int N::EarthLocation::getLatitude(QString latitude)
{
  unsigned int lati = 0;
  int d,l,m,s,v;
  char XLINE[2048];
  char D[64];
  D[0] = 'N';
  ::strcpy(XLINE,(const char *)latitude.toUtf8().constData());
  ::sscanf(XLINE,"%s %d %d' %d.%d",D,&l,&m,&s,&v);
  if (D[0]=='N') d = 1; else d = 0;
  if (l>=90) l = 90;
  if (d==1) lati |= 0x1;
  lati <<= 7 ;
  lati  |= l ;
  lati <<= 6 ;
  lati  |= m ;
  lati <<= 6 ;
  lati  |= s ;
  lati <<= 12;
  lati  |= v ;
  return lati;
}

unsigned long long N::EarthLocation::setLocation(double longitude,double latitude)
{
  unsigned int X                          ;
  unsigned int Y                          ;
  X          = getLongitude ( longitude ) ;
  Y          = getLatitude  ( latitude  ) ;
  location   = Y                          ;
  location <<= 32                         ;
  location  |= X                          ;
  return location                         ;
}

unsigned long long N::EarthLocation::setLocation(QString longitude,QString latitude)
{
  unsigned int X                          ;
  unsigned int Y                          ;
  X          = getLongitude ( longitude ) ;
  Y          = getLatitude  ( latitude  ) ;
  location   = Y                          ;
  location <<= 32                         ;
  location  |= X                          ;
  return location                         ;
}

QString N::EarthLocation::LongitudeString(void)
{
  QString      S,Q;
  unsigned int longi = (unsigned int)(location & 0x00000000FFFFFFFFLL);
  int          d,l,m,s,v;
  v = longi & 2047; longi >>= 11;
  s = longi &   63; longi >>= 6 ;
  m = longi &   63; longi >>= 6 ;
  l = longi &  255; longi >>= 8 ;
  d = longi;
  v *= 100; v /= 2048;
  S = "";
  if (d==0) Q = "E"; else Q = "W";
  S = Q;
  Q.sprintf("%d",l);
  S = S + " " + Q;
  Q.sprintf("%d",m);
  S = S + " " + Q + "'";
  Q.sprintf("%d.%d",s,v);
  S = S + " " + Q + "\"";
  return S;
}

QString N::EarthLocation::LatitudeString(void)
{
  QString      S,Q;
  unsigned int lati = (unsigned int)(location >> 32);
  int          d,l,m,s,v;
  v = lati & 4095; lati >>= 12;
  s = lati &   63; lati >>= 6 ;
  m = lati &   63; lati >>= 6 ;
  l = lati &  127; lati >>= 7 ;
  d = lati;
  v *= 100; v /= 4096;
  S = "";
  if (d==0) Q = "S"; else Q = "N";
  S = Q;
  Q.sprintf("%d",l);
  S = S + " " + Q;
  Q.sprintf("%d",m);
  S = S + " " + Q + "'";
  Q.sprintf("%d.%d",s,v);
  S = S + " " + Q + "\"";
  return S;
}

double N::EarthLocation::Longitude(void)
{
  unsigned int longi = (unsigned int)(location & 0x00000000FFFFFFFFLL);
  double       longf = 0.0f;
  int          d,l,m,s,v;
  v = longi & 2047; longi >>= 11;
  s = longi &   63; longi >>= 6 ;
  m = longi &   63; longi >>= 6 ;
  l = longi &  255; longi >>= 8 ;
  d = longi;
  longf  = (double)v; longf /= 2048.0f;
  longf += (double)s; longf /= 60.0f;
  longf += (double)m; longf /= 60.0f;
  longf += (double)l;
  if (d==0) longf = -longf;
  return longf;
}

double N::EarthLocation::Latitude(void)
{
  unsigned int lati = (unsigned int)(location >> 32);
  double       latf = 0.0f;
  int          d,l,m,s,v;
  v = lati & 4095; lati >>= 12;
  s = lati &   63; lati >>= 6 ;
  m = lati &   63; lati >>= 6 ;
  l = lati &  127; lati >>= 7 ;
  d = lati;
  latf  = (double)v; latf /= 4096.0f;
  latf += (double)s; latf /= 60.0f;
  latf += (double)m; latf /= 60.0f;
  latf += (double)l;
  if (d==0) latf = -latf;
  return latf;
}

unsigned int N::EarthLocation::getLongitude(double longitude)
{
  unsigned int longi = 0                                ;
  int          d,l,m,s,v                                ;
  if (longitude>=0.0f) d = 1; else d = 0                ;
  l = (int)longitude; longitude -= l; longitude *=   60 ;
  m = (int)longitude; longitude -= m; longitude *=   60 ;
  s = (int)longitude; longitude -= s; longitude *= 2048 ;
  v = (int)longitude                                    ;
  if (l>=180) l = 180                                   ;
  if (d==1) longi |= 0x1                                ;
  longi <<= 8                                           ;
  longi  |= l                                           ;
  longi <<= 6                                           ;
  longi  |= m                                           ;
  longi <<= 6                                           ;
  longi  |= s                                           ;
  longi <<= 11                                          ;
  longi  |= v                                           ;
  return longi                                          ;
}

unsigned int N::EarthLocation::getLatitude(double latitude)
{
  unsigned int lati = 0                              ;
  int          d,l,m,s,v                             ;
  if (latitude>=0.0f) d = 1; else d = 0              ;
  l = (int)latitude; latitude -= l; latitude *=   60 ;
  m = (int)latitude; latitude -= m; latitude *=   60 ;
  s = (int)latitude; latitude -= s; latitude *= 4096 ;
  v = (int)latitude                                  ;
  if (l>=90) l = 90                                  ;
  if (d==1) lati |= 0x1                              ;
  lati <<= 7                                         ;
  lati  |= l                                         ;
  lati <<= 6                                         ;
  lati  |= m                                         ;
  lati <<= 6                                         ;
  lati  |= s                                         ;
  lati <<= 12                                        ;
  lati  |= v                                         ;
  return lati                                        ;
}
