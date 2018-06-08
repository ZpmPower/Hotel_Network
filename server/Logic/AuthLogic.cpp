#include "AuthLogic.h"
#include "db/AuthPostgresManager.h"
#include "Config/GlobalParams.h"
#include "Crypto/CryptoHelper.h"
#include "db/HotelPostgresManager.h"
#include "Types/AuthUserInfo.h"
#include "Types/EmployeeInfo.h"
#include "Types/GuestInfo.h"
#include "Helper.h"
#include "db/SessionManagerPostgres.h"

ResponseCode AuthLogic::createUser(const network::RegisterMessage &authData, network::RegisterMessageResponse* response)//guest
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
        if(AuthPostgresManager::createUser(authData.login(), hashedPass, uniqSalt, user_id,authData.role()) != ResponseCode::status_success)
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

ResponseCode AuthLogic::createEmployee(const network::RegisterEmployeeMessage &authData, network::RegisterMessageResponse *response)
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
        if(AuthPostgresManager::createUser(authData.login(), hashedPass, uniqSalt, user_id, authData.role()) != ResponseCode::status_success)
        {
            if(response)
            {
                response->set_messagetext("Cannot create user! Try again");
                response->set_status(false);
            }
        }
        else
        {
            HotelPostgresManager::createEmployee(user_id, authData.firstname(), authData.secondname(), authData.lastname(), authData.phonenumber(),
                                                 authData.salary(),authData.position(),authData.hotelid());
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

ResponseCode AuthLogic::authUser(const network::AuthMessage &authData, network::AuthMessageResponse *response, network::SessionInfo *sessionInfo)
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
        AuthPostgresManager::getUser(authData.login(), uInfo);
        std::string passWithSalt = authData.pass() + GlobalsParams::getPostgres_default_solt();
        std::string uniqSalt = uInfo.salt;
        std::string hashedPass;
        std::string session;
        CryptoHelper::md5_hash(passWithSalt + uniqSalt, hashedPass);
        LOG_INFO(uInfo.pass);
        LOG_INFO(hashedPass);
        if(uInfo.pass == hashedPass)
        {
            bool isExist;
            SessionManagerPostgres::isExistSession(uInfo.user_id,isExist);
            if(isExist)
            {
                SessionManagerPostgres::removeSession(uInfo.user_id);
            }
            session = CryptoHelper::gen_random_string(25);
            SessionManagerPostgres::createSession(uInfo.user_id, session);
            sessionInfo->set_session_id(session);
            sessionInfo->set_login(uInfo.user_login);
            sessionInfo->set_userid(uInfo.user_id);
            sessionInfo->set_role(uInfo.role);

           switch (static_cast<Roles>(uInfo.role))
           {
           case Roles::role_guest:
           {
               ResponseCode resGuest = h_manager.getGuestInfo(uInfo.user_id,gInfo);
               response->set_messagetext("Succes Guest");
               break;
           }
           case Roles::role_admin:
           {
               response->set_messagetext("Succes Admin");
               break;
           }
           default:
           {
               ResponseCode resEmployee = h_manager.getEmployeeInfo(uInfo.user_id,eInfo);
               response->set_id_hotel(eInfo.hotel_id);
               response->set_id_user(eInfo.employee_id);
               break;
           }
           }
           response->set_role(static_cast<int32_t>(uInfo.role));
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
