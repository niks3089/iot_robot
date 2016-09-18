namespace IoTBot
{
    class DistanceSensorDriver
    {
    public:
        /**
          * @brief Class constructor.
          * @param distance The maximum distance in centimeters that needs to be tracked.
          */
        DistanceSensorDriver(unsigned int distance) : maxDistance(distance) {}
        
        /**
         * @brief Return the distance to the nearest obstacle in centimeters.
         * @return the distance to the closest object in centimeters 
         *   or maxDistance if no object was detected
         */
        virtual unsigned int getDistance() = 0;
        
    protected:
        unsigned int maxDistance;
    };
	
	class DistanceSensor : public DistanceSensorDriver
    {
    public:
        DistanceSensor(int triggerPin, int echoPin, int maxDistance)
            : DistanceSensorDriver(maxDistance),
              sensor(triggerPin, echoPin, maxDistance)
        {
        }

        virtual unsigned int getDistance()
        {
            int distance = sensor.ping_cm();
            if (distance <= 0)
                return maxDistance;
            return distance;
        }
    private:
        NewPing sensor;
    };

};
