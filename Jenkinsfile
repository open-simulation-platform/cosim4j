pipeline {
    agent none

     environment {
        CONAN_USER_HOME_SHORT = 'None'
        OSP_CONAN_CREDS = credentials('jenkins-osp-conan-creds')
    }

    stages {

        stage('Build') {
            parallel {
                stage('Build on Windows') {
                    agent { label 'windows' }

                    tools { jdk 'jdk8' }

                    environment {
                        CONAN_USER_HOME = "${env.SLAVE_HOME}/conan-repositories/${env.EXECUTOR_NUMBER}"
                    }

                    stages {
                        stage('Configure Conan') {
                            steps {
                                sh 'conan remote add osp https://osp-conan.azurewebsites.net/artifactory/api/conan/conan-local --force'
                                sh 'conan user -p $OSP_CONAN_CREDS_PSW -r osp $OSP_CONAN_CREDS_USR'
                            }
                        }
                        stage('Build-native') {
                            steps {
                                dir('build') {
                                   bat 'conan install ../native -s build_type=Release --build=missing'
                                   bat 'cmake -G "Visual Studio 15 2017 Win64" ../native'
                                   bat 'cmake --build . --config Release'
                                }
                            }
                        }
                        stage('Build-jvm') {
                            steps {
                                bat 'gradlew.bat clean build'
                            }
                        }

                    }

                }

                stage('Build on Linux') {
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
                                sh 'conan remote add osp https://osp-conan.azurewebsites.net/artifactory/api/conan/conan-local --force'
                                sh 'conan user -p $OSP_CONAN_CREDS_PSW -r osp $OSP_CONAN_CREDS_USR'
                            }
                        }
                        stage('Build-native') {
                            steps {
                                dir('build') {
                                    sh 'conan install ../native -s compiler.libcxx=libstdc++11 -s build_type=Release --build=missing'
                                    sh 'cmake -DCMAKE_BUILD_TYPE=Release ../native'
                                    sh 'cmake --build .'
                                }
                            }
                        }
                        stage('Build-jvm') {
                            steps {
                                sh './gradlew clean build'
                            }
                        }

                    }
                }
            }
        }
    }
}
