DROP TABLE IF EXISTS server_users CASCADE;
DROP TABLE IF EXISTS server_session CASCADE;

CREATE TABLE server_users (
    user_login text UNIQUE NOT NULL,
    password text NOT NULL,
    user_id BIGSERIAL NOT NULL,
    creation_time timestamp DEFAULT NOW(),
    salt text NOT NULL,
    role int NOT NULL,
    PRIMARY KEY(user_id)
);

CREATE TABLE server_session (
    session text NOT NULL,
    user_id BIGINT NOT NULL,
    end_date timestamp DEFAULT NOW() + '1 hour',
    PRIMARY KEY(session),
    FOREIGN KEY(user_id) REFERENCES server_users (user_id)
);


