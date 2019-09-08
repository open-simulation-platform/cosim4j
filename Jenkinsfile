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
                                   bat 'conan install ../native -s build_type=Release -build=missing'
                                   bat 'cmake ../native -G "Visual Studio 15 2017 Win64" ../'
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
                        }
                    }

                    stages {
                        stage('Build-native') {
                            steps {
                                sh 'cd native && cmake -H. -Bbuild -DCSECOREJNI_USING_CONAN=OFF'
                                sh 'cmake --build build'
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
