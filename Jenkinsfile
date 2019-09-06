pipeline {
    agent none

    stages {

        stage('Configure Conan') {
            steps {
                sh 'conan remote add osp https://osp-conan.azurewebsites.net/artifactory/api/conan/conan-local --force'
                sh 'conan remote add helmesjo https://api.bintray.com/conan/helmesjo/public-conan --force'
                sh 'conan user -p $OSP_CONAN_CREDS_PSW -r osp $OSP_CONAN_CREDS_USR'
            }
        }

        stage('Build') {
            parallel {
                stage('Windows') {
                    agent { label 'windows' }

                    tools { jdk 'jdk8' }
                    
                    steps {
                        bat 'conan install . -s build_type=Release --install-folder=build --build=missing'
                        bat 'gradlew.bat clean build'
                    }
                }

                stage ('Linux') {
                    agent { 
                        dockerfile {
                            filename 'Dockerfile.build'
                            dir '.dockerfiles'
                            label 'linux && docker'
                        }
                    }

                    steps {
                        sh 'conan install . -s build_type=Release -s compiler.libcxx=libstdc++11 --install-folder=build --build=missing'
                        sh './gradlew clean build'
                    }
                }
            }
        }
    }
}
