#include <qtgeography.h>

N::IpGeolocation:: IpGeolocation (void)
{
}

N::IpGeolocation::~IpGeolocation (void)
{
}

SUID N::IpGeolocation::toIP(QString IP)
{
  QStringList L = IP . split ( '.' )      ;
  if ( 4 != L . count ( ) ) return 0      ;
  SUID v = 0                              ;
  for (int i=0;i<4;i++)                   {
    bool okay = false                     ;
    int  x    = L [ i ] . toInt ( &okay ) ;
    if ( ! okay ) return 0                ;
    v <<= 8                               ;
    v  |= x                               ;
  }                                       ;
  return v                                ;
}

bool N::IpGeolocation::FindCities(QString IP,QList<QGeoLocation> & locations)
{
  SUID ip = toIP ( IP )                               ;
  if ( 0 == ip ) return false                         ;
  for (int i=2;i<31;i++)                              {
    int          s = 32 - i                           ;
    unsigned int mask = 0xFFFFFFFF                    ;
    SUID         M                                    ;
    SUID         P                                    ;
    mask >>= i                                        ;
    mask <<= i                                        ;
    M      = mask                                     ;
    P      = ( ip & M )                               ;
    if ( CityIpUuids . contains ( P ) )               {
      short m = CityIPs [ P ]                         ;
      if ( s == m )                                   {
        SUID ic = IpCities [ P ]                      ;
        if ( CityUuids . contains ( ic ) )            {
          QGeoLocation l = Cities [ ic ]              ;
          if ( IpCoordinates . contains ( P ) )       {
            l . setCoordinate ( IpCoordinates [ P ] ) ;
          }                                           ;
          locations << l                              ;
        }                                             ;
      }                                               ;
    }                                                 ;
  }                                                   ;
  return ( locations . count ( ) > 0 )                ;
}

bool N::IpGeolocation::FindCountries(QString IP,QList<QGeoLocation> & locations)
{
  SUID ip = toIP ( IP )                       ;
  if ( 0 == ip ) return false                 ;
  for (int i=2;i<31;i++)                      {
    int          s = 32 - i                   ;
    unsigned int mask = 0xFFFFFFFF            ;
    SUID         M                            ;
    SUID         P                            ;
    mask >>= i                                ;
    mask <<= i                                ;
    M      = mask                             ;
    P      = ( ip & M )                       ;
    if ( CountryIpUuids . contains ( P ) )    {
      short m = CountryIpUuids [ P ]          ;
      if ( s == m )                           {
        SUID ic = IpCountries [ P ]           ;
        if ( CountryUuids . contains ( ic ) ) {
          locations << Countries [ ic ]       ;
        }                                     ;
      }                                       ;
    }                                         ;
  }                                           ;
  return ( locations . count ( ) > 0 )        ;
}

bool N::IpGeolocation::FindAll(QString IP,QList<QGeoLocation> & locations)
{
  FindCities    ( IP , locations            ) ;
  FindCountries ( IP , locations            ) ;
  return        ( locations . count ( ) > 0 ) ;
}

QString N::IpGeolocation::Get(QByteArray & Data,int & index)
{
  if ( index < 0 ) return ""                        ;
  QByteArray b                                      ;
  QString    s    = ""                              ;
  int        next = Data . indexOf ( '\n' , index ) ;
  ///////////////////////////////////////////////////
  if ( next < 0 )                                   {
    int l = Data . size ( )                         ;
    if ( l > index )                                {
      l -= index                                    ;
      b  = Data . mid ( index , l )                 ;
      s  = QString::fromUtf8 ( b )                  ;
    }                                               ;
    index = -1                                      ;
    return s                                        ;
  }                                                 ;
  ///////////////////////////////////////////////////
  b     = Data . mid ( index , next - index )       ;
  s     = QString::fromUtf8 ( b )                   ;
  s     = s . replace ( '\r' , "" )                 ;
  s     = s . replace ( '\n' , "" )                 ;
  index = next + 1                                  ;
  ///////////////////////////////////////////////////
  return s                                          ;
}

bool N::IpGeolocation::FromTXT(QString filename)
{
  QByteArray B                                              ;
  if ( ! N::File::toByteArray(filename,B) ) return false    ;
  if ( B . size ( ) <= 0                  ) return false    ;
  ///////////////////////////////////////////////////////////
  int     step  = 0                                         ;
  int     index = 0                                         ;
  QString s                                                 ;
  ///////////////////////////////////////////////////////////
  do                                                        {
    s = Get ( B , index )                                   ;
    if ( s . length ( ) > 0 )                               {
      QString L = s                                         ;
      L = L . left ( 10 )                                   ;
      if ( ( 1 == ( step % 2 ) ) && ( "----------" == L ) ) {
        step ++                                             ;
      } else                                                {
        if ( 0 == ( step %2 ) )                             {
          step ++                                           ;
        } else                                              {
          QStringList    Z = s . split ( ',' )              ;
          QStringList    W                                  ;
          QGeoLocation   loca                               ;
          QGeoAddress    addr                               ;
          QGeoCoordinate coor                               ;
          QString        t                                  ;
          int            id                                 ;
          switch ( step )                                   {
            case 1                                          :
              // geoname_id,locale_code,continent_code,continent_name,country_iso_code,country_name
              if (  6 == Z . count ( ) )                    {
                id = Z [ 0 ] . toInt ( )                    ;
//                if ( ! CountryUuids . contains ( id ) )     {
                  addr   . clear          (         )       ;
                  addr   . setCountryCode ( Z [ 4 ] )       ;
                  if ( Z [ 5 ] . length ( ) <= 0 )          {
                    t = Z [ 3 ]                             ;
                    t = t . replace ( '"' , "" )            ;
                    addr . setCountry     ( t )             ;
                  } else                                    {
                    t = Z [ 5 ]                             ;
                    t = t . replace ( '"' , "" )            ;
                    addr . setCountry     ( t )             ;
                  }                                         ;
                  loca . setAddress ( addr )                ;
                  Countries [ id ] = loca                   ;
                  CountryUuids << id                        ;
//                }                                           ;
              }                                             ;
            break                                           ;
            case 3                                          :
              // network,geoname_id,registered_country_geoname_id,represented_country_geoname_id,is_anonymous_proxy,is_satellite_provider
              if (  6 == Z . count ( ) )                    {
                W = Z [ 0 ] . split ( '/' )                 ;
                if ( 2 == W . count ( ) )                   {
                  SUID ip = toIP ( W [ 0 ] )                ;
//                  if ( ! CountryIpUuids . contains ( ip ) ) {
                    short mask = W [ 1 ] . toShort ( )      ;
                    int   gid  = Z [ 1 ] . toInt   ( )      ;
                    CountryIPs  [ ip ] = mask               ;
                    IpCountries [ ip ] = gid                ;
                    CountryIpUuids    << ip                 ;
                  }                                         ;
//                }                                           ;
              }                                             ;
            break                                           ;
            case 5                                          :
              // geoname_id,locale_code,continent_code,continent_name,country_iso_code,country_name,subdivision_1_iso_code,subdivision_1_name,subdivision_2_iso_code,subdivision_2_name,city_name,metro_code,time_zone
              if ( 13 == Z . count ( ) )                    {
                id = Z [ 0 ] . toInt ( )                    ;
//                if ( ! CityUuids . contains ( id ) )        {
                  addr   . clear          (          )      ;
                  addr   . setCountryCode ( Z [  4 ] )      ;
                  if ( Z [  5 ] . length ( ) <= 0 )         {
                    t = Z [ 3 ]                             ;
                    t = t . replace ( '"' , "" )            ;
                    addr . setCountry     ( t )      ;
                  } else                                    {
                    t = Z [ 5 ]                             ;
                    t = t . replace ( '"' , "" )            ;
                    addr . setCountry     ( t )             ;
                  }                                         ;
                  if ( Z [  7 ] . length ( ) <= 0 )         {
                    t = Z [ 6 ]                             ;
                    t = t . replace ( '"' , "" )            ;
                    addr . setState       ( t )             ;
                  } else                                    {
                    t = Z [ 7 ]                             ;
                    t = t . replace ( '"' , "" )            ;
                    addr . setState       ( t )             ;
                  }                                         ;
                  if ( Z [  9 ] . length ( ) <= 0 )         {
                    t = Z [ 8 ]                             ;
                    t = t . replace ( '"' , "" )            ;
                    addr . setDistrict    ( t )             ;
                  } else                                    {
                    t = Z [ 9 ]                             ;
                    t = t . replace ( '"' , "" )            ;
                    addr . setDistrict    ( t )             ;
                  }                                         ;
                  t   = Z [ 10 ]                            ;
                  t   = t . replace ( '"' , "" )            ;
                  addr . setCity          ( t    )          ;
                  t   = Z [ 11 ]                            ;
                  t   = t . replace ( '"' , "" )            ;
                  addr . setPostalCode    ( t    )          ;
                  loca . setAddress       ( addr )          ;
                  Cities [ id ] = loca                      ;
                  CityUuids << id                           ;
//                }                                           ;
              }                                             ;
            break                                           ;
            case 7                                          :
              // network,geoname_id,registered_country_geoname_id,represented_country_geoname_id,is_anonymous_proxy,is_satellite_provider,postal_code,latitude,longitude
              if (  9 == Z . count ( ) )                    {
                W = Z [ 0 ] . split ( '/' )                 ;
                if ( 2 == W . count ( ) )                   {
                  SUID  ip   = toIP ( W [ 0 ] )             ;
//                  if ( ! CityIpUuids . contains ( ip ) )    {
                    short mask = W [ 1 ] . toShort ( )      ;
                    int   gid  = Z [ 1 ] . toInt   ( )      ;
                    int   nid  = Z [ 2 ] . toInt   ( )      ;
                    CityIPs   [ ip ] = mask                 ;
                    IpCities  [ ip ] = gid                  ;
                    IpNations [ ip ] = nid                  ;
                    CityIpUuids     << ip                   ;
                    if ( ( Z [ 7 ] . length ( ) > 0 )      &&
                         ( Z [ 8 ] . length ( ) > 0 )     ) {
                      double latitude  = Z [7] . toDouble() ;
                      double longitude = Z [8] . toDouble() ;
                      coor . setLatitude  ( latitude  )     ;
                      coor . setLongitude ( longitude )     ;
                      IpCoordinates [ ip ] = coor           ;
                    }                                       ;
//                  }                                         ;
                }                                           ;
              }                                             ;
            break                                           ;
          }                                                 ;
        }                                                   ;
      }                                                     ;
    }                                                       ;
  } while ( index > 0 )                                     ;
  ///////////////////////////////////////////////////////////
  B . clear ( )                                             ;
  return true                                               ;
}

bool N::IpGeolocation::FromDAT(QString filename)
{
  return false ;
}

bool N::IpGeolocation::FromNDB(QString filename)
{
  return false ;
}

bool N::IpGeolocation::ToDAT(QString filename)
{
  return false ;
}

bool N::IpGeolocation::ToNDB(QString filename)
{
  return false ;
}

bool N::IpGeolocation::ToSQL(SqlConnection & connection)
{
  return false ;
}
