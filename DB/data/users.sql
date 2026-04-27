-- Lack of password hashing
INSERT INTO users (id, username, password, role_id, active) VALUES
(1, 'admin', 'adminpass', 'admin', TRUE),
(2, 'user1', 'user1pass', 'user', TRUE),
(3, 'user2', 'user2pass', 'user', TRUE);