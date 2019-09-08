pipeline {
    agent none

     environment {
        CONAN_USER_HOME_SHORT = 'None'
        OSP_CONAN_CREDS = credentials('jenkins-osp-conan-creds')
        CSE_CONAN_CHANNEL = "${env.BRANCH_NAME}".take(51).replaceAll("/", "_")
    }

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
                                sh 'conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan --force'
                                sh 'conan user -p $OSP_CONAN_CREDS_PSW -r osp $OSP_CONAN_CREDS_USR'
                            }
                        }
                        stage('Build-native') {
                            steps {
                                bat 'cd native && conan install native -s build_type=Release --install-folder=build -build=missing'
                                bat 'cd native/build && cmake .. -A x64'
                                bat 'cd native/build && cmake --build .'
                            }
                        }
                        stage('Build-jvm') {
                            steps {
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
