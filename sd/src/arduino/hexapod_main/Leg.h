/**
 * @file Leg.h
 * @brief Implementation of Leg class
 * @author Radek Svec
 * Contact: xsvecr01@stud.fit.vutbr.cz
 */
 
#include "MyServo.h"

class Leg
{
    public:
        bool _right;
        LegPosition _legPos;
                
        Leg(MyServo *coxa, MyServo *femur, MyServo *tibia, LegPosition legPos, bool right = true)
        {
            _Coxa = coxa;
            _Femur = femur;
            _Tibia = tibia;
            _right = right;
            _legPos = legPos;
        }

        void SetXYZ(float x, float y, float z, float z_off, uint16_t duration, bool adjust = false)
        {
            z_off += 18;
            float z_dif = z_off - z;            
            float gamma = atan(x/y);
            float L1 = sqrt(x * x + y * y);
            float L = sqrt((z_dif * z_dif) + pow(L1 - COXA_LEN, 2));
            float alpha = acos(z_dif / L) + acos(((TIBIA_LEN * TIBIA_LEN) - (FEMUR_LEN * FEMUR_LEN) - (L * L)) / (-2 * FEMUR_LEN * L));
            float beta = acos(((L * L) - (TIBIA_LEN * TIBIA_LEN) - (FEMUR_LEN * FEMUR_LEN)) / (-2 * TIBIA_LEN * FEMUR_LEN));


            gamma = (int)(gamma * 180 / PI);
            alpha = (int)(alpha * 180 / PI);
            beta = (int)(beta * 180 / PI);

            if(_right)
            {
                _Coxa->SetPos(_Coxa->GetMid() - gamma, duration);
                _Femur->SetPos(_Femur->GetMid() - (alpha - 90), duration);
                _Tibia->SetPos(_Tibia->GetMid() + (beta - 90), duration);
            }
            else
            {
                _Coxa->SetPos(_Coxa->GetMid() + gamma, duration);
                _Femur->SetPos(_Femur->GetMid() + (alpha - 90), duration);
                _Tibia->SetPos(_Tibia->GetMid() - (beta - 90), duration);
            }
        }

        void Down(float height, uint16_t duration)
        {
            float gamma;
            float alpha;
            float beta;

            if(_right)
            {
                gamma = _Coxa->GetMid() - _Coxa->GetPos();
                alpha = _Femur->GetMid() - _Femur->GetPos() + 90;
                beta = - _Tibia->GetMid() + _Tibia->GetPos() + 90;
            }
            else
            {
                gamma = - _Coxa->GetMid() + _Coxa->GetPos();
                alpha = - _Femur->GetMid() + _Femur->GetPos() + 90;
                beta = _Tibia->GetMid() - _Tibia->GetPos() + 90;
            }

            gamma = gamma * (PI / 180);
            alpha = (90 - alpha) * (PI / 180);
            beta = (180 + beta) * (PI / 180);

            float y = (COXA_LEN + (FEMUR_LEN * cos(alpha)) + (TIBIA_LEN * cos(alpha - beta))) * cos(gamma);
            float x = (COXA_LEN + (FEMUR_LEN * cos(alpha)) + (TIBIA_LEN * cos(alpha - beta))) * sin(gamma);

            SetXYZ(x, y, 1, height, duration);
        }

        void Rotate(uint16_t angle, uint16_t duration)
        {
            _Coxa->SetPos(_Coxa->GetMid() + (angle - 90), duration);
            _Femur->Sleep(duration);
            _Tibia->Sleep(duration);
        }


        void Sleep(uint16_t duration)
        {
            _Coxa->Sleep(duration);
            _Femur->Sleep(duration);
            _Tibia->Sleep(duration);
        }

        void Middle(uint16_t duration)
        {
            _Coxa->SetPos(_Coxa->GetMid(), duration);
            _Femur->SetPos(_Femur->GetMid(), duration);
            _Tibia->SetPos(_Tibia->GetMid(), duration);
        }

        void Clear()
        {
            _Coxa->Clear();
            _Femur->Clear();
            _Tibia->Clear();
        }

        void FoldFast()
        {
            _Coxa->SetPosFast(_Coxa->GetDefault());
            if (_right)
            {
                _Femur->SetPosFast(_Femur->GetDefault() - 80);
                _Tibia->SetPosFast(_Tibia->GetDefault() - 80);
            }
            else
            {
                _Femur->SetPosFast(_Femur->GetDefault() + 80);
                _Tibia->SetPosFast(_Tibia->GetDefault() + 80);
            }
        }

        void Fold(uint16_t duration)
        {
            _Coxa->SetPos(_Coxa->GetDefault(), duration);
            if (_right)
            {
                _Femur->SetPos(_Femur->GetDefault() - 80, duration);
                _Tibia->SetPos(_Tibia->GetDefault() - 80, duration);
            }
            else
            {
                _Femur->SetPos(_Femur->GetDefault() + 80, duration);
                _Tibia->SetPos(_Tibia->GetDefault() + 80, duration);
            }
        }

        bool Finished(int items)
        {
            if(_Coxa->QueueFinished(items) && _Femur->QueueFinished(items) && _Tibia->QueueFinished(items))
            {
                return true;
            }
            else
            {
                return false;
            }  
        }

        bool IsMiddle()
        {
            if(_Coxa->GetPos() == _Coxa->GetMid() && _Femur->GetPos() == _Femur->GetMid() && _Tibia->GetPos() == _Tibia->GetMid())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    private:
        MyServo *_Coxa;
        MyServo *_Femur;
        MyServo *_Tibia;
};
