-- tables without foreign keys
CREATE TABLE roles (
    id VARCHAR(255) PRIMARY KEY,
    description TEXT NOT NULL,
    active BOOLEAN DEFAULT TRUE
);

CREATE TABLE permissions (
    id SERIAL PRIMARY KEY,
    description TEXT NOT NULL,
    active BOOLEAN DEFAULT TRUE
);

-- tables with foreign keys
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(255) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role_id VARCHAR(255) NOT NULL,
    active BOOLEAN DEFAULT TRUE,
    CONSTRAINT fk_user FOREIGN KEY (role_id) REFERENCES roles(id)
);

CREATE TABLE role_permissions (
    id SERIAL PRIMARY KEY,
    role_id VARCHAR(255) NOT NULL,
    permission_id INT NOT NULL,
    CONSTRAINT fk_role FOREIGN KEY (role_id) REFERENCES roles(id),
    CONSTRAINT fk_permission FOREIGN KEY (permission_id) REFERENCES permissions(id)
);
