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
                                dir('cpp/cse-core4j') {
                                   bat 'conan install . -s build_type=Release --install-folder=build --build=missing'
                                   dir('build') {
                                       bat 'cmake -G "Visual Studio 15 2017 Win64" ..'
                                       bat 'cmake --build . --config Release'
                                   }
                                }
                            }
                        }
                        stage('Build-jvm') {
                            steps {
								dir('java') {
									bat 'gradlew.bat clean build'
								}
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
                                dir('cpp/cse-core4j') {
                                    sh 'conan install . -s compiler.libcxx=libstdc++11 -s build_type=Release --install-folder=build --build=missing'
                                    dir ('build') {
                                        sh 'cmake -DCMAKE_BUILD_TYPE=Release ..'
                                        sh 'cmake --build .'
                                    }
                                }
                            }
                        }
                        stage('Build-jvm') {
                            steps {
                                dir('java') {
									sh './gradlew clean build'
								}
                            }
                        }
                    }
                }
            }
        }
    }
}
