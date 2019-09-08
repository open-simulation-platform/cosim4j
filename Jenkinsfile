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
                                sh 'conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan --force'
                                sh 'conan user -p "Open Simulation Platform" -r osp osp'
                            }
                        }
                        stage('Build') {
                            steps {
                                dir('native/build'){
                                    bat 'conan install .. -s build_type=Release -build=missing'
                                    bat 'cmake .. -A x64'
                                    bat 'cmake --build .'
                                }
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
                        stage('Build') {
                            steps {
                                dir('native/build') {
                                    sh 'cmake -H. -Bbuild -DCSECOREJNI_USING_CONAN=OFF'
                                    sh 'cmake --build build'
                                }
                                sh './gradlew clean build'
                            }
                        }

                    }
                }
            }
        }
    }
}
