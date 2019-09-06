pipeline {
    agent none

    stages {

        stage('Build') {
            parallel {
                stage('Windows') {
                    agent { label 'windows' }

                    tools { jdk 'jdk8' }

                    environment {
                        CONAN_USER_HOME = "${env.SLAVE_HOME}/conan-repositories/${env.EXECUTOR_NUMBER}"
                    }

                    stages {
                        stage('Configure Conan') {
                            steps {
                                sh 'conan remote add osp https://osp-conan.azurewebsites.net/artifactory/api/conan/conan-local --force'
                                sh 'conan remote add helmesjo https://api.bintray.com/conan/helmesjo/public-conan --force'
                                sh 'conan user -p "Open Simulation Platform" -r osp osp'
                            }
                        }

                        stage('Build') {
                            steps {
                                bat 'conan install . -s build_type=Release --install-folder=build --build=missing'
                                bat 'gradlew.bat clean build'
                            }
                        }

                    }

                }

                stage('Linux') {
                    agent {
                        dockerfile {
                            filename 'Dockerfile.build'
                            dir '.dockerfiles'
                            label 'linux && docker'
                            args '-v ${SLAVE_HOME}/conan-repositories/${EXECUTOR_NUMBER}:/conan_repo'
                        }
                    }

                    environment {
                        CONAN_USER_HOME = '/conan_repo'
                    }

                    stages {

                        stage('Configure Conan') {
                            steps {
                                sh 'conan --version'
                                sh 'cmake --version'
                                sh 'conan remote add osp https://osp-conan.azurewebsites.net/artifactory/api/conan/conan-local --force'
                                sh 'conan remote add helmesjo https://api.bintray.com/conan/helmesjo/public-conan --force'
                                sh 'conan user -p "Open Simulation Platform" -r osp osp'
                            }
                        }

                        stage('Build') {
                            steps {
                                sh 'conan install . -s build_type=Release -s compiler.libcxx=libstdc++11 --install-folder=build --build=missing'
                                sh './gradlew copyNativeLibs'
                                sh './gradlew clean build'
                            }
                        }

                    }
                }
            }
        }
    }
}
