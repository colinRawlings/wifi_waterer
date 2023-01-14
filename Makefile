mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

dev_python = $(mkfile_dir)/.venv/bin/python
dev_pre_commit = $(mkfile_dir)/.venv/bin/pre-commit


info:
	echo $(mkfile_path)
	echo $(dir $(mkfile_path))

devenv:
	python3 -m venv --clear $(mkfile_dir)/.venv
	$(dev_python) -m pip install -U pip setuptools
	$(dev_python) -m pip install -U pre-commit cmake-format
	$(dev_pre_commit) install
