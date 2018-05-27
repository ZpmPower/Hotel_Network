#include "AuthLogic.h"
#include "db/AuthPostgresManager.h"
#include "Config/GlobalParams.h"
#include "Crypto/CryptoHelper.h"
#include "db/HotelPostgresManager.h"
#include "Types/AuthUserInfo.h"
#include "Types/EmployeeInfo.h"
#include "Types/GuestInfo.h"
#include "Helper.h"

ResponseCode AuthLogic::createUser(const network::RegisterMessage &authData, network::RegisterMessageResponse* response)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        bool isExist = false;
        result = AuthPostgresManager::isUserExist(authData.login(), isExist);

        if(result != ResponseCode::status_success)
        {
            LOG_ERR("Error checking if user exist!");
            if(response)
            {
                response->set_messagetext("Something wrong!");
                response->set_status(false);
            }
            break;
        }

        if(isExist)
        {
            LOG_ERR("User already exist!");
            if(response)
            {
                response->set_messagetext("Same user already exist!");
                response->set_status(false);
            }
            break;
        }

        std::string passWithSalt = authData.pass() + GlobalsParams::getPostgres_default_solt();
        std::string uniqSalt = CryptoHelper::gen_random_string(30);
        std::string hashedPass;
        CryptoHelper::md5_hash(passWithSalt + uniqSalt, hashedPass);
        LOG_INFO(authData.pass());
        int64_t user_id;
        if(AuthPostgresManager::createUser(authData.login(), hashedPass, uniqSalt, user_id) != ResponseCode::status_success)
        {
            if(response)
            {
                response->set_messagetext("Cannot create user! Try again");
                response->set_status(false);
            }
        }
        else
        {
            HotelPostgresManager::createGuest(user_id, authData.firstname(), authData.secondname(), authData.lastname(), authData.phonenumber(), authData.passport());
            if(response)
            {
                response->set_messagetext("Success");
                response->set_status(true);
            }
        }


    }
    while(false);

    return result;
}

ResponseCode AuthLogic::authUser(const network::AuthMessage &authData, network::AuthMessageResponse *response)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        bool isExist = false;
        result = AuthPostgresManager::isUserExist(authData.login(), isExist);

        if(result != ResponseCode::status_success)
        {
            LOG_ERR("Error checking if user exist!");
            if(response)
            {
                response->set_messagetext("Something wrong!");
                response->set_status(false);
            }
            break;
        }

        AuthUserInfo uInfo;
        GuestInfo gInfo;
        EmployeeInfo eInfo;
        HotelPostgresManager h_manager;
        if(uInfo.user_login == "admin")
        {

        }
        AuthPostgresManager::getUser(authData.login(), uInfo);
        std::string passWithSalt = authData.pass() + GlobalsParams::getPostgres_default_solt();
        std::string uniqSalt = uInfo.salt;
        std::string hashedPass;
        CryptoHelper::md5_hash(passWithSalt + uniqSalt, hashedPass);
        LOG_INFO(uInfo.pass);
        LOG_INFO(hashedPass);
        if(uInfo.pass == hashedPass)
        {
           ResponseCode resEmployee = h_manager.getEmployeeInfo(uInfo.user_id,eInfo);
           ResponseCode resGuest = h_manager.getGuestInfo(uInfo.user_id,gInfo);
           Roles role;
           if(resEmployee == ResponseCode::status_does_not_exist)
           {
               if(resGuest == ResponseCode::status_does_not_exist)
               {
                    role = Helper::roleToInt("Admin");
                    response->set_messagetext("Succes Admin");
               }
               else
               {
                   role = Helper::roleToInt("Guest");
                   response->set_messagetext("Succes Guest");
               }
           }
           else
           {
                role = Helper::roleToInt(eInfo.position);
                if(role == Helper::roleToInt("Manager"))
                {
                    response->set_messagetext("Succes Manager");
                }
                if(role == Helper::roleToInt("Receptionist"))
                {
                    response->set_messagetext("Succes Receptionist");
                }
                response->set_id_hotel(eInfo.hotel_id);

           }
           LOG_INFO("Manager=" + std::to_string(static_cast<int32_t>(Roles::role_manager)) + "\n");
           LOG_INFO("Admin=" + std::to_string(static_cast<int32_t>(Roles::role_admin)) + "\n");
           LOG_INFO("Guest=" + std::to_string(static_cast<int32_t>(Roles::role_guest)) + "\n");
           LOG_INFO("Recept=" + std::to_string(static_cast<int32_t>(Roles::role_receptionist)) + "\n");
           response->set_role(static_cast<int32_t>(role));
           response->set_status(true);
        }
        else
        {
            LOG_ERR("Error while auth");
            if(response)
            {
                response->set_messagetext("User isn't registered");
                response->set_status(false);
                response->set_role(1000);
            }
            break;
        }

    }
    while(false);

    return result;
}
