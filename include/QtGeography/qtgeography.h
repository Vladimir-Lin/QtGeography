/****************************************************************************
 *                                                                          *
 * Copyright (C) 2015 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_GEOGRAPHY_H
#define QT_GEOGRAPHY_H

#include <QtCore>
#include <QtNetwork>
#include <QtSql>
#include <QtScript>
#include <QtPositioning>
#include <QtLocation>
#include <Essentials>
#include <Mathematics>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTGEOGRAPHY_LIB)
#      define Q_GEOGRAPHY_EXPORT Q_DECL_EXPORT
#    else
#      define Q_GEOGRAPHY_EXPORT Q_DECL_IMPORT
#    endif
#else
#      define Q_GEOGRAPHY_EXPORT
#endif

namespace N
{

class Q_GEOGRAPHY_EXPORT Geolocation   ;
class Q_GEOGRAPHY_EXPORT EarthLocation ;
class Q_GEOGRAPHY_EXPORT IpGeolocation ;

/*****************************************************************************
 *                                                                           *
 *                           Geographic Operations                           *
 *                                                                           *
 *****************************************************************************/

class Q_GEOGRAPHY_EXPORT Geolocation
{
  public:

    double longitude ;
    double latitude  ;
    double height    ;

    explicit Geolocation (void);
    virtual ~Geolocation (void);

  protected:

  private:

};

class Q_GEOGRAPHY_EXPORT EarthLocation
{
  public:

    unsigned long long location ;

    explicit EarthLocation          (void) ;
             EarthLocation          (double longitude,double latitude) ;
    virtual ~EarthLocation          (void) ;

    unsigned int       setLongitude (double  longitude);
    unsigned int       setLatitude  (double  latitude );
    unsigned int       getLongitude (double  longitude);
    unsigned int       getLatitude  (double  latitude );
    unsigned int       getLongitude (QString longitude);
    unsigned int       getLatitude  (QString latitude );
    unsigned long long setLocation  (double  longitude,double  latitude);
    unsigned long long setLocation  (QString longitude,QString latitude);

    QString LongitudeString         (void);
    QString LatitudeString          (void);

    double Longitude                (void);
    double Latitude                 (void);

  protected:

  private:

};

class Q_GEOGRAPHY_EXPORT IpGeolocation
{
  public:

    QMap<SUID,short         > CountryIPs     ;
    QMap<SUID,SUID          > IpCountries    ;
    UUIDs                     CountryIpUuids ;
    //////////////////////////////////////////
    QMap<SUID,short         > CityIPs        ;
    QMap<SUID,SUID          > IpNations      ;
    QMap<SUID,SUID          > IpCities       ;
    QMap<SUID,QGeoCoordinate> IpCoordinates  ;
    UUIDs                     CityIpUuids    ;
    //////////////////////////////////////////
    QMap<SUID,QGeoLocation  > Countries      ;
    UUIDs                     CountryUuids   ;
    //////////////////////////////////////////
    QMap<SUID,QGeoLocation  > Cities         ;
    UUIDs                     CityUuids      ;

    explicit     IpGeolocation (void) ;
    virtual     ~IpGeolocation (void) ;

    virtual bool FromTXT       (QString filename) ;
    virtual bool FromDAT       (QString filename) ;
    virtual bool FromNDB       (QString filename) ;
    virtual bool ToDAT         (QString filename) ;
    virtual bool ToNDB         (QString filename) ;
    virtual bool ToSQL         (SqlConnection & connection) ;

    virtual bool FindCities    (QString IP,QList<QGeoLocation> & locations) ;
    virtual bool FindCountries (QString IP,QList<QGeoLocation> & locations) ;
    virtual bool FindAll       (QString IP,QList<QGeoLocation> & locations) ;

  protected:

    SUID         toIP          (QString IP) ;
    QString      Get           (QByteArray & Data,int & index) ;

  private:

};

typedef QMap<SUID,EarthLocation> EGCSs ;

}

Q_DECLARE_METATYPE(N::Geolocation)
Q_DECLARE_METATYPE(N::EarthLocation)
Q_DECLARE_METATYPE(N::EGCSs)

QT_END_NAMESPACE

#endif
