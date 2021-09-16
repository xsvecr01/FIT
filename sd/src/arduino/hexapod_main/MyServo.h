/**
 * @file MyServo.h
 * @brief Implementation of MyServo class
 * @author Radek Svec
 * Contact: xsvecr01@stud.fit.vutbr.cz
 */
 
#include "headers.h"

class MyServo
{
    public:
        MyServo(uint8_t pin, Adafruit_PWMServoDriver *pca, uint8_t channel = 0)
        {
            if (!pca)
            {
                ledcSetup(channel, FREQ, RESOLUTION);
                ledcAttachPin(pin, channel);
            }
            _pin = pin;
            pcaBoard = pca;
            _channel = channel;
            _startMillis = millis();

            _prevAngle = _angle = _default = _mid = 90;

            commandQ = xQueueCreate(16, sizeof(struct Command));
        }

        uint8_t GetMid()
        {
            return _mid;
        }

        uint8_t GetDefault()
        {
            return _default;
        }

        void SetMid(uint8_t value)
        {
            _mid =  value;
        }

        void SetDefault(uint8_t value)
        {
            _angle = value;
            _default = value;
            _mid = value;
            _prevAngle = value;
        }

        void Sleep(uint16_t duration)
        {
            SetPos(_lastInserted, duration);
        }


        void SetPos(uint8_t angle, float duration)
        {
            _lastInserted = angle;
            Command cmd;
            cmd.angle = angle;
            cmd.duration = duration;
            xQueueSend(commandQ, &cmd, 10);
        }

        void SetPosFast(uint8_t angle)
        {
            _SetPos(angle);
            _angle = _prevAngle = _desired = _lastInserted = angle;
        }

        void Refresh(unsigned long currM)
        {       
            if(currM - _startMillis >= _duration)
            {
                _startMillis = currM;
                _angle = _desired;
                _prevAngle = _angle;

                Command cmd;

                if(xQueueReceive(commandQ, &cmd, 0))
                {
                    _desired = cmd.angle;
                    _duration = cmd.duration;
                }
            }
            else if(_desired != _angle)
            {
                _angle = _prevAngle + (_desired - _prevAngle) * ((currM - _startMillis) / _duration);
            }
            if(uxQueueMessagesWaiting(commandQ) == 0 && _lastInserted == _angle && _desired == _angle)
                _startMillis = currM;
            _SetPos(_angle);
        }

        bool QueueFinished(int items)
        {
            if(items == 0)
            {
                if(uxQueueMessagesWaiting(commandQ) == 0 && _lastInserted == _angle && _desired == _angle)
                    return true;
            }
            else if(uxQueueMessagesWaiting(commandQ) <= items)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void Clear()
        {
            vQueueDelete(commandQ);
            commandQ = xQueueCreate(16, sizeof(struct Command));
            _desired = _angle;
        }

        int GetPos()
        {
            return _angle;
        }

    private:
        Adafruit_PWMServoDriver *pcaBoard;
        QueueHandle_t commandQ;
        
        uint8_t _pin, _channel, _mid, _default, _angle, _prevAngle, _desired, _lastInserted;
        float _duration = 400;
        unsigned long _startMillis;
        bool _pca;
        

        struct Command
        {
            uint8_t angle;
            float duration;
        };

        
        float _GetDuty(uint8_t angle)
        {
            float duty = (TEN_EIGHTY * angle + 2);
            return ((4095 * duty) / 100);
        }

        void _SetPos(uint8_t angle)
        {
            if (!pcaBoard)
            {
                ledcWrite(_channel, _GetDuty(angle));
            }
            else
            {
                pcaBoard->setPWM(_pin, 0, _GetDuty(angle));
            }
            _angle = angle;
        }
};
