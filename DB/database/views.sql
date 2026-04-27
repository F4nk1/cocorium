CREATE OR REPLACE VIEW active_users AS
SELECT * FROM users WHERE active = TRUE;
