DELIMITER $$


/* SeedUsers - 
   Add users to the users table */
CREATE PROCEDURE SeedUsers(IN users_to_insert INT)
BEGIN
    /* Declare stored procedure variables */
    DECLARE next_id INT;
    DECLARE i INT DEFAULT 0;
	
    /* Set stored procedure variables */
    IF (SELECT COUNT(id) FROM users) = 0 THEN
        SET next_id = 1;
    ELSE
        SET next_id = (SELECT MAX(id) FROM users) + 1;
    END IF;
    
    user_loop: LOOP
		
        /* Leave loop if all users have been inserted */
        IF i = users_to_insert THEN
            LEAVE user_loop;
        END IF;
    
        /* Insert new user into users table */
        INSERT INTO users(first_name, last_name, email)
            VALUES('seed', 
                   CONCAT('user_', next_id), 
                   CONCAT('seed.user_', next_id, '@email.com'));
	
        /* Increment i and next_id variables */
        SET i = i + 1;
        SET next_id = next_id + 1;
		
    END LOOP;
END $$


/* GetFollowIds - 
   Utility to generate two random user ids */
CREATE PROCEDURE GetFollowIds(OUT follower_id INT, OUT followee_id INT)
BEGIN

    /* Calculate number of users in table */
    DECLARE max_id INT;
    SET max_id = (SELECT MAX(id) FROM users);

    /* Generate random integer between 1 and max_id */
    SET follower_id = FLOOR(RAND() * (max_id) + (1));
    SET followee_id = FLOOR(RAND() * (max_id) + (1));

    /* If ids are the same, generate a new followee_id */
    WHILE follower_id = followee_id DO
        SET followee_id = FLOOR(RAND() * (max_id) + (1));
    END WHILE;
END $$


/* SeedFollowings - 
   Generate followings between users */
CREATE PROCEDURE SeedFollowings(IN iterations INT)
BEGIN

    /* We must generate two user ids to attempt a following */
    DECLARE follower_id INT;
    DECLARE followee_id INT;
    DECLARE i INT DEFAULT 0;
	
    outer_while: WHILE i < iterations DO
        REPEAT
            /* Call another stored procedure to get a user id */
            CALL GetFollowIds(follower_id, followee_id);
            
            inner_while: WHILE ((SELECT COUNT(id) FROM users WHERE id=follower_id) = 0 OR 
                                (SELECT COUNT(id) FROM users WHERE id=followee_id) = 0) DO
                CALL GetFollowIds(follower_id, followee_id);
            END WHILE inner_while;
            
        UNTIL ((SELECT COUNT(*) FROM followings WHERE follower=follower_id AND followee=followee_id) = 0)
        END REPEAT;
	
        /* Insert new following in database */
        INSERT INTO followings(follower, followee) VALUES(follower_id, followee_id);
		
        /* Increment iterator */
        SET i = i + 1;
        
        /* Display new following ids */
        SELECT follower_id, followee_id;
    END WHILE outer_while;
END $$


DELIMITER ;
