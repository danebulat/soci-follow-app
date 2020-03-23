/* Create and use database  */
CREATE DATABASE social_db;
USE social_db;

/* Create a new user and set user's grants */
CREATE USER developer@localhost IDENTIFIED BY 'Secure123';
GRANT ALL ON social_db.* TO developer@localhost;
GRANT EXECUTE ON ocial_db.* TO developer@localhost;
SET DEFAULT ROLE ALL TO developer@localhost;

/* Verify the developer user's grants */
SHOW GRANTS FOR developer@localhost;

/* Users table */
CREATE TABLE users (
    id INT AUTO_INCREMENT,
    email VARCHAR(128) UNIQUE NOT NULL,
    first_name VARCHAR(128) NOT NULL,
    last_name VARCHAR(128) NOT NULL,
    active BOOL DEFAULT TRUE,
    PRIMARY KEY(id)
) ENGINE=INNODB;

/* Messages Table */
CREATE TABLE messages (
    id INT AUTO_INCREMENT,
    to_id INT NOT NULL,
    from_id INT NOT NULL,
    message TEXT NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY (to_id) REFERENCES users(id) 
        ON UPDATE CASCADE 
        ON DELETE CASCADE
) ENGINE=INNODB;

/* Followings Table */
CREATE TABLE followings (
    follower INT NOT NULL,
    followee INT NOT NULL,
    PRIMARY KEY(follower, followee),
    FOREIGN KEY (follower) REFERENCES users(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    FOREIGN KEY (followee) REFERENCES users(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
) ENGINE=INNODB;

