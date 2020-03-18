/* Create and use database  */
CREATE DATABASE social_db;
USE social_db;

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

