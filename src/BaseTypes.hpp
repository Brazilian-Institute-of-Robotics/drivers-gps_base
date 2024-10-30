#ifndef _GPS_BASE_BASETYPES_HPP_
#define _GPS_BASE_BASETYPES_HPP_

#include <vector>
#include <base/Time.hpp>
#include <base/Pose.hpp>

namespace gps_base
{
    enum GPS_SOLUTION_TYPES
    {
        NO_SOLUTION    = 0, //! No GPS solution found
        AUTONOMOUS_2D  = 6, //! Like AUTONOMOUS, but solution doesn't provide height information. Is 6 for historical reasons
        AUTONOMOUS     = 1, //! Solution not using differential corrections
        DIFFERENTIAL   = 2, //! Atmospheric correction is used e.g. as provided by a Satellite or Ground Based Augmentation System
        INVALID        = 3, //! Correction result is invalid
        RTK_FIXED      = 4, //! RTK solution
        RTK_FLOAT      = 5  //! RTK solution with undetermined phase shift
    };

    /** @deprecated do not use in new code */
    struct Time {
        base::Time cpu_time;
        base::Time gps_time;
        double processing_latency = base::unknown<double>();
    };

    /** Representation of a position returned by a GNSS device, in WGS84 */
    struct Solution {
        base::Time time;
        /** Latitude in degrees
         *
         * It is in degrees for historical reasons
         */
        double latitude = base::unknown<double>();
        /** Longitude in degrees
         *
         * It is in degrees for historical reasons
         */
        double longitude = base::unknown<double>();
        /** Type of solution */
        GPS_SOLUTION_TYPES positionType;
        /** How many satellites are used in this solution */
        int noOfSatellites = 0;
        /** Altitude above mean sea level */
        double altitude = base::unknown<double>();
        /** Geoidal separation at this location
         *
         * The difference between the WGS-84 earth ellipsoid and mean-sea-level
         * (geoid), "-" means mean-sea-level below ellipsoid
         */
        double geoidalSeparation = base::unknown<double>();
        /** Age of differential corrections used in this solution if any
         *
         * Set to base::unknown<double> if no differential corrections are available
         */
        double ageOfDifferentialCorrections = base::unknown<double>();

        /** Error on the east-west axis, in meters
         */
        double deviationLatitude = base::unknown<double>();
        /** Error on the north-south axis, in meters
         */
        double deviationLongitude = base::unknown<double>();
        /** Error in altitude, in meters
         */
        double deviationAltitude = base::unknown<double>();

        Solution()
            : positionType(INVALID) {}
    };

    struct Position {
        base::Time time;
        double latitude = base::unknown<double>();
        double longitude = base::unknown<double>();
        GPS_SOLUTION_TYPES positionType;
        int noOfSatellites = 0;
        double altitude = base::unknown<double>();
        double geoidalSeparation = base::unknown<double>();
        double ageOfDifferentialCorrections = base::unknown<double>();
        Position()
            : positionType(INVALID) {}
    };

    struct Errors {
        base::Time time;
        double deviationLatitude = base::unknown<double>();
        double deviationLongitude = base::unknown<double>();
        double deviationAltitude = base::unknown<double>();
    };

    struct SolutionQuality {
        base::Time time;
        std::vector<int> usedSatellites;
        double pdop = base::unknown<double>();
        double hdop = base::unknown<double>();
        double vdop = base::unknown<double>();
    };

    enum CONSTELLATIONS {
        CONSTELLATION_GPS,
        CONSTELLATION_SBAS,
        CONSTELLATION_GLONASS,
        CONSTELLATION_GALILEO,
        CONSTELLATION_BEIDOU,
        CONSTELLATION_QZSS
    };
    struct Satellite {
        int PRN;
        int elevation;
        int azimuth;
        double SNR = base::unknown<double>();

        static CONSTELLATIONS getConstellationFromPRN(int prn)
        {
            if (prn < 33)
                return CONSTELLATION_GPS;
            else if ((prn < 65) || ((prn >= 152) && (prn <= 158)))
                return CONSTELLATION_SBAS;
            else if ((prn >= 301) && (prn <= 336))
                return CONSTELLATION_GALILEO;
            else if ((prn >= 401) && (prn <= 437))
                return CONSTELLATION_BEIDOU;
            else if ((prn >= 193) && (prn <= 202))
                return CONSTELLATION_QZSS;
            else
                return CONSTELLATION_GLONASS;
        }

        CONSTELLATIONS getConstellation() const
        {
            return getConstellationFromPRN(PRN);
        }
    };

    struct SatelliteInfo {
        base::Time time;
        std::vector < gps_base::Satellite> knownSatellites;
    };

    /** @deprecated do not use in new code */
    struct UserDynamics {
        int hSpeed;
        int hAccel;
        int vSpeed;
        int vAccel;
        UserDynamics()
            : hSpeed(0), hAccel(0), vSpeed(0), vAccel(0) {}
    };

    /** Full quality information about the solution */
    struct ConstellationInfo {
        gps_base::SolutionQuality quality;
        gps_base::SatelliteInfo  satellites;
    };

    /**
     * Set of parameters used to setup GPS-to-local cartesian coordinates
     * conversion
     */
    struct UTMConversionParameters {
        /** The local origin for UTM-to-NWU conversion
         */
        base::Position nwu_origin = base::Position::Zero();
        /** UTM zone number
         */
        int utm_zone = 1;
        /** North or south of the equator
         */
        bool utm_north = true;
    };

}

#endif // _GPS_BASE_BASETYPES_HPP_
