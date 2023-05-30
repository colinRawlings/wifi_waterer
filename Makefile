makefile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
makefile_dir := $(patsubst %/,%,$(dir $(makefile_path)))

FRONTEND_DIR = ${makefile_dir}/frontend
BACKEND_DIR = ${makefile_dir}/backend
BACKEND_VENV_DIR = ${BACKEND_DIR}/.venv
EMBEDDED_DIR = ${makefile_dir}/embedded

EMBEDDED_TESTBUILD_DIR = ${EMBEDDED_DIR}/build

backend_startup_script := $(makefile_dir)/launch_backend.sh
frontend_startup_script := $(makefile_dir)/launch_frontend.sh

SERVER_IP = 192.168.8.133
SERVER_USER = ubuntu
SERVER_TIMEZONE = Europe/Zurich

ip_config_filepath = $(makefile_dir)/ip_config.json

ifndef
THIS_IP = $(firstword $(shell hostname -I))
endif

ifdef OS
	COMMENT_CHAR = REM
	BASE_PYTHON = py -3.8
	BACKEND_VENV_PYTHON = ${BACKEND_VENV_DIR}/Scripts/python.exe
	BACKEND_VENV_PIP_SYNC = ${BACKEND_VENV_DIR}/Scripts/pip-sync.exe
	RENAME_CMD = rename
	ACTIVATE_CMD = ${BACKEND_VENV_DIR}/Scripts/activate
else
	SHELL=/bin/bash
	COMMENT_CHAR = \#
	BASE_PYTHON = python3
	BACKEND_VENV_PYTHON = ${BACKEND_VENV_DIR}/bin/python
	BACKEND_VENV_PIP_SYNC = ${BACKEND_VENV_DIR}/bin/pip-sync
	RENAME_CMD = mv
	ACTIVATE_CMD = source ${BACKEND_VENV_DIR}/bin/activate
endif

.PHONY: info venv

info:
	@echo makefile_dir: ${makefile_dir}
	@echo venv_python: ${BACKEND_VENV_PYTHON}
	@echo IP: ${THIS_IP}

venv: ${BACKEND_VENV_DIR}

${BACKEND_VENV_DIR}:
	${BASE_PYTHON} -m venv --clear ${BACKEND_VENV_DIR}
	${BACKEND_VENV_PYTHON} -m pip install -U pip setuptools wheel
	${BACKEND_VENV_PYTHON} -m pip install -r ${BACKEND_DIR}/requirements/base.txt

requirements: venv
	${BACKEND_VENV_PYTHON} -m pip install pip-tools
	${BACKEND_VENV_PYTHON} -m piptools compile --resolver=backtracking ${BACKEND_DIR}/requirements/base.in
	${BACKEND_VENV_PYTHON} -m piptools compile --resolver=backtracking ${BACKEND_DIR}/requirements/dev.in
	${COMMENT_CHAR} Call install(-dev) to update ...

update-reqs-dev: | requirements
	${BACKEND_VENV_PIP_SYNC} ${BACKEND_DIR}/requirements/dev.txt
	${BACKEND_VENV_PYTHON} -m pip install -e ${BACKEND_DIR}
	cd ${BACKEND_DIR} && ${ACTIVATE_CMD} && pyright --verbose


install-ssh:
	sudo apt update
	sudo apt install -y openssh-server
	sudo systemctl enable ssh
	sudo systemctl start ssh

install-host-tools: install-ssh
ifdef OS
	${COMMENT_CHAR} TODO: node, npm, yarn, angular, arduino
else
	# This probably only works on ubuntu
	sudo apt update
	#
	sudo apt install -y \
		build-essential \
		gcc \
		make \
		clang-format \
		software-properties-common
	# node
	curl -fsSL https://deb.nodesource.com/setup_16.x | sudo -E bash -
	sudo apt install -y nodejs
	sudo npm install -g -y yarn
	sudo npm install -g -y lite-server
endif

install-host-dev-tools: install-host-tools
	# This probably only works on debian
	sudo apt update
	# installing arduino IDE
	sudo snap install arduino
	sudo npm install -g -y @angular/cli


install-dev: | install-host-dev-tools venv
	${COMMENT_CHAR} Python Tools
	${BACKEND_VENV_PYTHON} -m pip install pip-tools
	${BACKEND_VENV_PIP_SYNC} ${BACKEND_DIR}/requirements/dev.txt
	${COMMENT_CHAR} Formatting
	${BACKEND_VENV_PYTHON} -m pre_commit install --install-hooks
	${COMMENT_CHAR} Install backend
	${BACKEND_VENV_PYTHON} -m pip install -e ${BACKEND_DIR}
	${COMMENT_CHAR} npm install -g pyright@1.1.240
	${COMMENT_CHAR} Install Frontend
	cd ${FRONTEND_DIR} && yarn install --production=false
	cd ${FRONTEND_DIR}/node_modules/@types && ${RENAME_CMD} plotly.js plotly.js-dist ; exit 0

install: | install-host-tools venv
	${COMMENT_CHAR} Install Backend
	${BACKEND_VENV_PYTHON} -m pip install pip-tools
	${BACKEND_VENV_PIP_SYNC} ${BACKEND_DIR}/requirements/base.txt
	${BACKEND_VENV_PYTHON} -m pip install -e ${BACKEND_DIR}
	${COMMENT_CHAR} Install Frontend
	cd ${FRONTEND_DIR} && yarn install --production=true
	${COMMENT_CHAR} For pi run: make install-service

install-services: | backend_startup_script frontend_startup_script
	sudo cp ${makefile_dir}/waterer_backend.service /etc/systemd/system/waterer_backend.service
	sudo systemctl enable waterer_backend
	sudo cp ${makefile_dir}/waterer_frontend.service /etc/systemd/system/waterer_frontend.service
	sudo systemctl enable waterer_frontend


uninstall-services:
	sudo systemctl stop waterer_backend
	sudo systemctl disable waterer_backend
	sudo systemctl stop waterer_frontend
	sudo systemctl disable waterer_frontend
	systemctl daemon-reload
	systemctl reset-failed

set-timezone:
	sudo timedatectl set-timezone ${SERVER_TIMEZONE}
	timedatectl

#

push-frontend:
	cd ${FRONTEND_DIR} && ng build
	rm -f ${FRONTEND_DIR}/dist/waterer/env.js
ifdef OS
	${COMMENT_CHAR} Run: wsl scp -r ${FRONTEND_DIR}/dist  $(SERVER_USER)@$(SERVER_IP):/home/ubuntu/wifi_waterer/frontend
else
	scp -r ${FRONTEND_DIR}/dist  $(SERVER_USER)@$(SERVER_IP):/home/ubuntu/wifi_waterer/frontend
endif
	# don't forget to make waterer-shell && cd wifi_waterer && git pull && make restart-services

#

backend_startup_script: ${backend_startup_script}

${backend_startup_script}:
	echo "#!/bin/sh" > ${backend_startup_script}
	echo "set -eux" >> ${backend_startup_script}
	echo "cd $(makefile_dir) && $(shell which make) -f $(makefile_dir)/Makefile up-backend" >> ${backend_startup_script}
	chmod +x ${backend_startup_script}

frontend_startup_script: ${frontend_startup_script}

${frontend_startup_script}:
	echo "#!/bin/sh" > ${frontend_startup_script}
	echo "set -eux" >> ${frontend_startup_script}
	echo "cd $(makefile_dir) && $(shell which make) -f $(makefile_dir)/Makefile up-frontend" >> ${frontend_startup_script}
	chmod +x ${frontend_startup_script}

#

up-frontend-dev: venv
	${BACKEND_VENV_PYTHON} ${FRONTEND_DIR}/make_templates.py
	cd ${FRONTEND_DIR} && yarn start

# useful for the rpi that lacks the power to build the frontend
up-frontend: venv
	${BACKEND_VENV_PYTHON} ${FRONTEND_DIR}/make_templates.py
	cd ${FRONTEND_DIR} && lite-server


up-backend-dev: export WATERER_FAKE_DATA=1
up-backend-dev:
	${BACKEND_VENV_PYTHON} -m waterer_backend.WiFi.run_server

up-backend:
	${BACKEND_VENV_PYTHON} -m waterer_backend.WiFi.run_server

tests-backend:
	${BACKEND_VENV_PYTHON} -m pytest ${makefile_dir}/backend/tests
	cd ${BACKEND_DIR} && ${ACTIVATE_CMD} && pyright
	cd ${BACKEND_DIR} && \
	 ${ACTIVATE_CMD} && \
	 python -m vulture vulture_whitelist.py ./waterer_backend --exclude="waterer_backend/models.py"

tests-embedded:
	mkdir -p ${EMBEDDED_TESTBUILD_DIR}
	cd ${EMBEDDED_TESTBUILD_DIR} && cmake -S .. &&  cmake --build . -j
	${EMBEDDED_TESTBUILD_DIR}/tests/test_runner

# waterer service
.PHONY: waterer-shell restart-services up-status

up-backend-service:
	sudo systemctl start waterer_backend.service

up-frontend-service:
	sudo systemctl start waterer_frontend.service

up-services: | up-backend-service up-frontend-service

#

down-backend-service:
	sudo systemctl stop waterer_backend.service

down-frontend-service:
	sudo systemctl stop waterer_frontend.service

down-services: | down-backend-service down-frontend-service

#

up-backend-status:
	sudo journalctl -u waterer_backend.service -f

up-frontend-status:
	sudo journalctl -u waterer_frontend.service -f

#

# clean up any dangling connections
restart-backend-service:
	sudo systemctl restart waterer_backend.service

restart-frontend-service:
	sudo systemctl restart waterer_frontend.service

restart-services: | restart-frontend-service restart-backend-service

#

waterer-shell:
	ssh $(SERVER_USER)@$(SERVER_IP)


pip-list:
	${BACKEND_VENV_PYTHON} -m pip list
